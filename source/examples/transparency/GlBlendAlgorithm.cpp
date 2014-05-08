#include "GlBlendAlgorithm.h"

#include <cassert>

#include <glow/Program.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>

#include <glowutils/File.h>
#include <glowutils/Camera.h>
#include <glowutils/glowutils.h>

void GlBlendAlgorithm::initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) {
    assert(vertexShader != nullptr);

	m_program = new glow::Program();
	m_program->attach(glowutils::createShaderFromFile(gl::FRAGMENT_SHADER, transparencyShaderFilePath + "glblend.frag"));
    m_program->attach(vertexShader);
	if (geometryShader != nullptr) m_program->attach(geometryShader);

    m_colorTex = createColorTex();
    m_depthBuffer = new glow::RenderBufferObject();

	m_fbo = new glow::FrameBufferObject();
    m_fbo->attachTexture2D(gl::COLOR_ATTACHMENT0, m_colorTex);
    m_fbo->attachRenderBuffer(gl::DEPTH_ATTACHMENT, m_depthBuffer);
    m_fbo->setDrawBuffer(gl::COLOR_ATTACHMENT0);
}

void GlBlendAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
    m_fbo->bind();

    gl::Viewport(0, 0, width, height);


    camera->setViewport(width, height);

    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);


    m_program->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_program->setUniform("normalmatrix", camera->normal());
    m_program->setUniform("screenSize", glm::vec2(width, height));
    m_program->use();

    gl::Enable(gl::BLEND);

    gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);


    drawFunction(m_program);

    gl::Disable(gl::BLEND);


    m_fbo->unbind();
}

void GlBlendAlgorithm::resize(int width, int height) {
	int depthBits = glow::FrameBufferObject::defaultFBO()->getAttachmentParameter(gl::DEPTH, gl::FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
    m_colorTex->image2D(0, gl::RGBA32F, width, height, 0, gl::RGBA, gl::FLOAT, nullptr);
    m_depthBuffer->storage(depthBits == 16 ? gl::DEPTH_COMPONENT16 : gl::DEPTH_COMPONENT, width, height);
}

glow::Texture* GlBlendAlgorithm::getOutput()
{
    return m_colorTex;
}
