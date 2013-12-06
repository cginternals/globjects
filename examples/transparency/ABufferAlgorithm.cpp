#include "ABufferAlgorithm.h"

#include <glow/Program.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>
#include <glow/Buffer.h>

#include <glowutils/File.h>
#include <glowutils/Camera.h>

namespace glow {

namespace {

struct ABufferEntry {
    glm::vec4 color;
    float z;
    unsigned int next;
};

} // anonymous namespace

void ABufferAlgorithm::initialize() {
    m_program = new glow::Program();
    m_program->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/abuffer.frag"));
    m_program->attach(glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/transparency/abuffer.vert"));

    m_colorTex = createColorTex();
    m_depthBuffer = new glow::RenderBufferObject();

    m_fbo = new glow::FrameBufferObject();
    m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_colorTex.get());
    m_fbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, m_depthBuffer.get());
    m_fbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);

    m_linkedListBuffer = new glow::Buffer(GL_SHADER_STORAGE_BUFFER);
    m_linkedListBuffer->setName("A Buffer Linked Lists");

    m_headBuffer = new glow::Buffer(GL_SHADER_STORAGE_BUFFER);
    m_headBuffer->setName("A Buffer Heads");

    m_counter = new glow::Buffer(GL_ATOMIC_COUNTER_BUFFER);
    m_counter->setName("A Buffer Counter");
}

void ABufferAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
    m_fbo->bind();

    glViewport(0, 0, width, height);
    camera->setViewport(width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CheckGLError();

    // reset head buffer & counter
    std::vector<int> initialHead(width * height, -1);
    m_headBuffer->setData(width * height * sizeof(int), &initialHead[0], GL_DYNAMIC_DRAW);
    int counterValue = 0;
    m_counter->setData(1 * sizeof(int), &counterValue, GL_DYNAMIC_DRAW);

    // bind buffers
    m_linkedListBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    m_headBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
    m_counter->bindBase(GL_ATOMIC_COUNTER_BUFFER, 0);

    m_program->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_program->setUniform("normalmatrix", camera->normal());
    m_program->setUniform("screenSize", glm::vec2(width, height));
    m_program->use();

    drawFunction(m_program.get());

    m_fbo->unbind();
}

void ABufferAlgorithm::resize(int width, int height) {
    int depthBits = FrameBufferObject::defaultFBO()->getAttachmentParameter(GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
    m_colorTex->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_depthBuffer->storage(depthBits == 16 ? GL_DEPTH_COMPONENT16 : GL_DEPTH_COMPONENT, width, height);
    m_linkedListBuffer->setData(width * height * 3 * sizeof(ABufferEntry), nullptr, GL_DYNAMIC_DRAW);
}

} // namespace glow
