#include "HybridAlgorithm.h"

#include <glow/Program.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>
#include <glow/Buffer.h>
#include <glow/NamedStrings.h>

#include <glowutils/File.h>
#include <glowutils/Camera.h>
#include <glowutils/ScreenAlignedQuad.h>

namespace glow {

namespace {

const int ABUFFER_SIZE = 4;

}

void HybridAlgorithm::initialize() {
    Shader* vertexShader = glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/transparency/hybrid.vert");

    m_opaqueProgram = new Program;
    m_opaqueProgram->attach(vertexShader);
    m_opaqueProgram->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/hybrid_opaque.frag"));

    m_depthKTabProgram = new Program;
    m_depthKTabProgram->attach(vertexShader);
    m_depthKTabProgram->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/hybrid_depthktab.frag"));

    m_depthBuffer = new RenderBufferObject;
    m_opaqueBuffer = createColorTex();
    m_depthKTab = new Buffer(GL_SHADER_STORAGE_BUFFER);

    m_fbo = new FrameBufferObject;
    m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_opaqueBuffer.get());
    m_fbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, m_depthBuffer.get());
    m_fbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);

    m_quad = new glowutils::ScreenAlignedQuad(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/hybrid_post.frag"));
    m_colorBuffer = createColorTex();
    m_postFbo = new glow::FrameBufferObject;
    m_postFbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_colorBuffer.get());
    m_postFbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void HybridAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
    glViewport(0, 0, width, height);
    camera->setViewport(width, height);

    m_fbo->bind();
    m_fbo->clear(GL_DEPTH_BUFFER_BIT);
    m_fbo->clearBuffer(GL_COLOR, 0, glm::vec4(1.0f));

    //
    // render opaque geometry
    //
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    m_opaqueProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_opaqueProgram->setUniform("normalmatrix", camera->normal());
    m_opaqueProgram->use();

    drawFunction(m_opaqueProgram.get());

    //
    // render translucent geometry into depth k-TAB
    //
    glDepthMask(GL_FALSE);

    static std::vector<unsigned int> initialDepthKTab;
    initialDepthKTab.resize(width * height * ABUFFER_SIZE, std::numeric_limits<unsigned int>::max());
    m_depthKTab->setData(initialDepthKTab.size() * sizeof(unsigned int), initialDepthKTab.data(), GL_DYNAMIC_DRAW);
    m_depthKTab->bindBase(GL_SHADER_STORAGE_BUFFER, 0);

    m_depthKTabProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_depthKTabProgram->setUniform("normalmatrix", camera->normal());
    m_depthKTabProgram->setUniform("screenSize", glm::ivec2(width, height));
    m_depthKTabProgram->use();

    drawFunction(m_depthKTabProgram.get());

    glDepthMask(GL_TRUE);

    m_fbo->unbind();

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    m_postFbo->bind();
    m_postFbo->clear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    m_opaqueBuffer->bind(GL_TEXTURE0);

    m_quad->program()->setUniform("screenSize", glm::ivec2(width, height));
    m_quad->program()->setUniform("opaqueBuffer", 0);
    m_quad->draw();

    m_opaqueBuffer->unbind(GL_TEXTURE0);

    m_postFbo->unbind();
}

void HybridAlgorithm::resize(int width, int height) {
    m_depthBuffer->storage(GL_DEPTH_COMPONENT, width, height);
    m_opaqueBuffer->image2D(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    m_colorBuffer->image2D(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

} // namespace glow
