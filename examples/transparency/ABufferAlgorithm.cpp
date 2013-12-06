#include "ABufferAlgorithm.h"

#include <glow/Program.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>
#include <glow/Buffer.h>

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

} // anonymous namespace

void ABufferAlgorithm::initialize() {
    m_renderProgram = new glow::Program();
    m_renderProgram->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/abuffer.frag"));
    m_renderProgram->attach(glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/transparency/abuffer.vert"));

    m_renderColorBuffer = createColorTex();
    m_renderDepthBuffer = new glow::RenderBufferObject();

    m_renderFbo = new glow::FrameBufferObject();
    m_renderFbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_renderColorBuffer.get());
    m_renderFbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, m_renderDepthBuffer.get());
    m_renderFbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);

    m_linkedListBuffer = new glow::Buffer(GL_SHADER_STORAGE_BUFFER);
    m_linkedListBuffer->setName("A Buffer Linked Lists");

    m_headBuffer = new glow::Buffer(GL_SHADER_STORAGE_BUFFER);
    m_headBuffer->setName("A Buffer Heads");

    m_counter = new glow::Buffer(GL_ATOMIC_COUNTER_BUFFER);
    m_counter->setName("A Buffer Counter");

    m_postQuad = new glowutils::ScreenAlignedQuad(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/abuffer_post.frag"));

    m_postColorBuffer = createColorTex();
    m_postFbo = new glow::FrameBufferObject;
    m_postFbo->clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_postFbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_postColorBuffer.get());
    m_postFbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void ABufferAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
    m_renderFbo->bind();

    glViewport(0, 0, width, height);
    camera->setViewport(width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CheckGLError();

    // reset head buffer & counter
    std::vector<int> initialHead(width * height, -1);
    m_headBuffer->setData(width * height * sizeof(int), &initialHead[0], GL_DYNAMIC_DRAW);
    static int initialCounter = 0;
    m_counter->setData(1 * sizeof(int), &initialCounter, GL_DYNAMIC_DRAW);

    // bind buffers
    m_linkedListBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    m_headBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
    m_counter->bindBase(GL_ATOMIC_COUNTER_BUFFER, 0);

    m_renderProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_renderProgram->setUniform("normalmatrix", camera->normal());
    m_renderProgram->setUniform("screenSize", glm::ivec2(width, height));
    m_renderProgram->use();

    drawFunction(m_renderProgram.get());

    m_renderFbo->unbind();

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    m_postFbo->bind();
    m_postFbo->clear(GL_COLOR_BUFFER_BIT);
    m_postQuad->program()->setUniform("screenSize", glm::ivec2(width, height));
    m_postQuad->draw();
    m_postFbo->unbind();
}

void ABufferAlgorithm::resize(int width, int height) {
    int depthBits = FrameBufferObject::defaultFBO()->getAttachmentParameter(GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
    m_renderColorBuffer->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_renderDepthBuffer->storage(depthBits == 16 ? GL_DEPTH_COMPONENT16 : GL_DEPTH_COMPONENT, width, height);
    m_linkedListBuffer->setData(width * height * 6 * sizeof(ABufferEntry), nullptr, GL_DYNAMIC_DRAW);
    m_postColorBuffer->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
}

} // namespace glow
