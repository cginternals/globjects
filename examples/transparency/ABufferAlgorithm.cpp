#include "ABufferAlgorithm.h"

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

void ABufferAlgorithm::initialize() {
    glow::NamedStrings::createNamedString("/transparency/abuffer_definitions", "const int ABUFFER_SIZE = " + std::to_string(ABUFFER_SIZE) + ";");
    glow::NamedStrings::createNamedString("/transparency/abuffer.glsl", new glowutils::File("data/transparency/abuffer.glsl"));

    m_program = new glow::Program();
    m_program->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/abuffer.frag"));
    m_program->attach(glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/transparency/abuffer.vert"));

    m_opaqueBuffer = createColorTex();
    m_depthBuffer = new glow::RenderBufferObject();

    m_renderFbo = new glow::FrameBufferObject();
    m_renderFbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_opaqueBuffer.get());
    m_renderFbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, m_depthBuffer.get());
    m_renderFbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);

    m_linkedListBuffer = new glow::Buffer(GL_SHADER_STORAGE_BUFFER);
    m_linkedListBuffer->setName("A Buffer Linked Lists");

    m_headBuffer = new glow::Buffer(GL_SHADER_STORAGE_BUFFER);
    m_headBuffer->setName("A Buffer Heads");

    m_counter = new glow::Buffer(GL_ATOMIC_COUNTER_BUFFER);
    m_counter->setName("A Buffer Counter");

    m_quad = new glowutils::ScreenAlignedQuad(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/abuffer_post.frag"));

    m_colorBuffer = createColorTex();
    m_postFbo = new glow::FrameBufferObject;
    m_postFbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_colorBuffer.get());
    m_postFbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void ABufferAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
    m_renderFbo->bind();
    m_renderFbo->clear(GL_DEPTH_BUFFER_BIT);
    m_renderFbo->clearBuffer(GL_COLOR, 0, glm::vec4(1.0f, 1.0f, 1.0f, std::numeric_limits<float>::max()));

    glViewport(0, 0, width, height);
    camera->setViewport(width, height);

    // reset head buffer & counter
    static std::vector<Head> initialHead;
    initialHead.resize(width * height);
    m_headBuffer->setData(width * height * sizeof(Head), &initialHead[0], GL_DYNAMIC_DRAW);
    static int initialCounter = 0;
    m_counter->setData(1 * sizeof(int), &initialCounter, GL_DYNAMIC_DRAW);

    // bind buffers
    m_linkedListBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    m_headBuffer->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
    m_counter->bindBase(GL_ATOMIC_COUNTER_BUFFER, 0);

    m_program->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_program->setUniform("normalmatrix", camera->normal());
    m_program->setUniform("screenSize", glm::ivec2(width, height));
    m_program->use();

    drawFunction(m_program.get());

    m_renderFbo->unbind();

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    m_postFbo->bind();
    m_postFbo->clear(GL_COLOR_BUFFER_BIT);

    m_opaqueBuffer->bind(GL_TEXTURE0);

    m_quad->program()->setUniform("screenSize", glm::ivec2(width, height));
    m_quad->program()->setUniform("opaqueBuffer", 0);
    m_quad->draw();

    m_opaqueBuffer->unbind(GL_TEXTURE0);

    m_postFbo->unbind();
}

void ABufferAlgorithm::resize(int width, int height) {
    int depthBits = FrameBufferObject::defaultFBO()->getAttachmentParameter(GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
    m_opaqueBuffer->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_depthBuffer->storage(depthBits == 16 ? GL_DEPTH_COMPONENT16 : GL_DEPTH_COMPONENT, width, height);
    m_linkedListBuffer->setData(width * height * ABUFFER_SIZE * sizeof(ABufferEntry), nullptr, GL_DYNAMIC_DRAW);
    m_colorBuffer->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
}

} // namespace glow