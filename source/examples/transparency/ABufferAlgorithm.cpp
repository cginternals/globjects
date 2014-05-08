#include "ABufferAlgorithm.h"

#include <glbinding/functions.h>

#include <glow/Program.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>
#include <glow/Buffer.h>
#include <glow/glow.h>
#include <glow/NamedString.h>

#include <glowutils/File.h>
#include <glowutils/Camera.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/glowutils.h>

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

void ABufferAlgorithm::initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) {
    glow::NamedString::create("/transparency/abuffer_definitions", "const int ABUFFER_SIZE = " + std::to_string(ABUFFER_SIZE) + ";");
    glow::NamedString::create("/transparency/abuffer.glsl", new glowutils::File(transparencyShaderFilePath + "abuffer.glsl"));

    m_program = new glow::Program();
	m_program->attach(glowutils::createShaderFromFile(gl::FRAGMENT_SHADER, transparencyShaderFilePath +  "abuffer.frag"));
	m_program->attach(vertexShader);
	if (geometryShader != nullptr) m_program->attach(geometryShader);

    m_opaqueBuffer = createColorTex();
    m_depthBuffer = new glow::RenderBufferObject();

    m_renderFbo = new glow::FrameBufferObject();
    m_renderFbo->attachTexture2D(gl::COLOR_ATTACHMENT0, m_opaqueBuffer.get());
    m_renderFbo->attachRenderBuffer(gl::DEPTH_ATTACHMENT, m_depthBuffer.get());
    m_renderFbo->setDrawBuffer(gl::COLOR_ATTACHMENT0);

    m_linkedListBuffer = new glow::Buffer();
    m_linkedListBuffer->setName("A Buffer Linked Lists");

    m_headBuffer = new glow::Buffer();
    m_headBuffer->setName("A Buffer Heads");

    m_counter = new glow::Buffer();
    m_counter->setName("A Buffer Counter");

	m_quad = new glowutils::ScreenAlignedQuad(glowutils::createShaderFromFile(gl::FRAGMENT_SHADER, transparencyShaderFilePath +  "abuffer_post.frag"));

    m_colorBuffer = createColorTex();
    m_postFbo = new glow::FrameBufferObject;
    m_postFbo->attachTexture2D(gl::COLOR_ATTACHMENT0, m_colorBuffer.get());
    m_postFbo->setDrawBuffer(gl::COLOR_ATTACHMENT0);
}

void ABufferAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
    m_renderFbo->bind();
    m_renderFbo->clear(gl::DEPTH_BUFFER_BIT);
    m_renderFbo->clearBuffer(gl::COLOR, 0, glm::vec4(1.0f, 1.0f, 1.0f, std::numeric_limits<float>::max()));

    // reset head buffer & counter
    static glm::ivec2 initialHead(-1, 0);
    static int initialCounter = 0;
    m_headBuffer->setData(static_cast<gl::GLsizei>(width * height * sizeof(Head)), nullptr, gl::DYNAMIC_DRAW);
    m_headBuffer->clearData(gl::RG32I, gl::RG, gl::INT, &initialHead);
    m_counter->setData(static_cast<gl::GLsizei>(sizeof(int)), nullptr, gl::DYNAMIC_DRAW);
    m_counter->clearData(gl::R32I, gl::RED, gl::UNSIGNED_INT, &initialCounter);

    // bind buffers
    m_linkedListBuffer->bindBase(gl::SHADER_STORAGE_BUFFER, 0);
    m_headBuffer->bindBase(gl::SHADER_STORAGE_BUFFER, 1);
    m_counter->bindBase(gl::ATOMIC_COUNTER_BUFFER, 0);

    m_program->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_program->setUniform("normalmatrix", camera->normal());
    m_program->setUniform("screenSize", glm::ivec2(width, height));
    m_program->use();

    drawFunction(m_program);

    m_renderFbo->unbind();

    gl::MemoryBarrier(gl::SHADER_STORAGE_BARRIER_BIT);


    m_postFbo->bind();
    m_postFbo->clear(gl::COLOR_BUFFER_BIT);

    m_opaqueBuffer->bindActive(gl::TEXTURE0);

    m_quad->program()->setUniform("screenSize", glm::ivec2(width, height));
    m_quad->program()->setUniform("opaqueBuffer", 0);
    m_quad->draw();

    m_opaqueBuffer->unbindActive(gl::TEXTURE0);

    m_postFbo->unbind();
}

void ABufferAlgorithm::resize(int width, int height) {
    int depthBits = glow::FrameBufferObject::defaultFBO()->getAttachmentParameter(gl::DEPTH, gl::FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
    m_opaqueBuffer->image2D(0, gl::RGBA32F, width, height, 0, gl::RGBA, gl::FLOAT, nullptr);
    m_depthBuffer->storage(depthBits == 16 ? gl::DEPTH_COMPONENT16 : gl::DEPTH_COMPONENT, width, height);
    m_linkedListBuffer->setData(static_cast<gl::GLsizei>(width * height * ABUFFER_SIZE * sizeof(ABufferEntry)), nullptr, gl::DYNAMIC_DRAW);
    m_colorBuffer->image2D(0, gl::RGBA32F, width, height, 0, gl::RGBA, gl::FLOAT, nullptr);
}

glow::Texture* ABufferAlgorithm::getOutput()
{
    return m_colorBuffer;
}
