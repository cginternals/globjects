#include "WeightedAverageAlgorithm.h"

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

struct ABufferEntry {
    glm::vec4 color;
    float z;
    int next;
};

struct Head {
    int startIndex;
    int size;

    Head()
    : startIndex(-1)
    , size(0) {
    }
};

const int ABUFFER_SIZE = 8;

} // anonymous namespace

void WeightedAverageAlgorithm::initialize() {
    glow::Shader* vertexShader = glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/transparency/wavg.vert");

    m_opaqueProgram = new glow::Program;
    m_opaqueProgram->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/wavg_opaque.frag"));
    m_opaqueProgram->attach(vertexShader);

    m_accumulationProgram = new glow::Program;
    m_accumulationProgram->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/wavg_translucent.frag"));
    m_accumulationProgram->attach(vertexShader);

    m_opaqueBuffer = createColorTex();
    m_accumulationBuffer = createColorTex();
    m_depthBuffer = new glow::RenderBufferObject();

    m_depthComplexityBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER);

    m_renderFbo = new glow::FrameBufferObject();
    m_renderFbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_opaqueBuffer.get());
    m_renderFbo->attachTexture2D(GL_COLOR_ATTACHMENT1, m_accumulationBuffer.get());
    m_renderFbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, m_depthBuffer.get());
    m_renderFbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 });

    m_quad = new glowutils::ScreenAlignedQuad(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/wavg_post.frag"));

    m_colorBuffer = createColorTex();
    m_postFbo = new glow::FrameBufferObject;
    m_postFbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_colorBuffer.get());
    m_postFbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void WeightedAverageAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
    static float clearDepth = 1.0f;
    m_renderFbo->bind();
    m_renderFbo->clearBuffer(GL_COLOR, 0, glm::vec4(1.0f)); // clear opaque buffer with background
    m_renderFbo->clearBuffer(GL_COLOR, 1, glm::vec4(0.0f)); // clear accumulation buffer with 0
    m_renderFbo->clearBufferfv(GL_DEPTH, 0, &clearDepth);

    glViewport(0, 0, width, height);
    camera->setViewport(width, height);

    // reset depth complexity
    std::vector<unsigned int> initialDepthComplexity(width * height, 0);
    m_depthComplexityBuffer->setData(width * height * sizeof(unsigned int), initialDepthComplexity.data(), GL_DYNAMIC_DRAW);
    m_depthComplexityBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    m_opaqueProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_opaqueProgram->setUniform("normalmatrix", camera->normal());
    m_opaqueProgram->use();

    drawFunction(m_opaqueProgram.get());

    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    m_accumulationProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_accumulationProgram->setUniform("normalmatrix", camera->normal());
    m_accumulationProgram->setUniform("screenSize", glm::ivec2(width, height));
    m_accumulationProgram->use();

    drawFunction(m_accumulationProgram.get());

    m_renderFbo->unbind();

    m_postFbo->bind();
    m_postFbo->clear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    m_opaqueBuffer->bind(GL_TEXTURE0);
    m_accumulationBuffer->bind(GL_TEXTURE1);

    m_quad->program()->setUniform("opaqueBuffer", 0);
    m_quad->program()->setUniform("accumulationBuffer", 1);
    m_quad->program()->setUniform("screenSize", glm::ivec2(width, height));

    m_quad->draw();

    m_opaqueBuffer->unbind(GL_TEXTURE0);
    m_accumulationBuffer->unbind(GL_TEXTURE1);

    glEnable(GL_DEPTH_TEST);

    m_postFbo->unbind();
}

void WeightedAverageAlgorithm::resize(int width, int height) {
    m_opaqueBuffer->image2D(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    m_accumulationBuffer->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_colorBuffer->image2D(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    m_depthBuffer->storage(GL_DEPTH_COMPONENT, width, height);
}

} // namespace glow
