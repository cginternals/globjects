#include "GlBlendAlgorithm.h"

#include <cassert>

#include <glbinding/gl/gl.h>

#include <globjects/Program.h>
#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>
#include <globjects/RenderBufferObject.h>

#include <globjects-base/File.h>
#include <globjects-utils/Camera.h>
#include <globjects-utils/globjects-utils.h>

void GlBlendAlgorithm::initialize(const std::string & transparencyShaderFilePath, glo::Shader *vertexShader, glo::Shader *geometryShader) {
    assert(vertexShader != nullptr);

	m_program = new glo::Program();
	m_program->attach(glo::Shader::fromFile(gl::GL_FRAGMENT_SHADER, transparencyShaderFilePath + "glblend.frag"));
    m_program->attach(vertexShader);
	if (geometryShader != nullptr) m_program->attach(geometryShader);

    m_colorTex = createColorTex();
    m_depthBuffer = new glo::RenderBufferObject();

	m_fbo = new glo::Framebuffer();
    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_colorTex);
    m_fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depthBuffer);
    m_fbo->setDrawBuffer(gl::GL_COLOR_ATTACHMENT0);
}

void GlBlendAlgorithm::draw(const DrawFunction& drawFunction, gloutils::Camera* camera, int width, int height) {
    m_fbo->bind();

    gl::glViewport(0, 0, width, height);


    camera->setViewport(width, height);

    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);


    m_program->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_program->setUniform("normalmatrix", camera->normal());
    m_program->setUniform("screenSize", glm::vec2(width, height));
    m_program->use();

    gl::glEnable(gl::GL_BLEND);

    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);


    drawFunction(m_program);

    gl::glDisable(gl::GL_BLEND);


    m_fbo->unbind();
}

void GlBlendAlgorithm::resize(int width, int height) {
	int depthBits = glo::Framebuffer::defaultFBO()->getAttachmentParameter(gl::GL_DEPTH, gl::GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
    m_colorTex->image2D(0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
    m_depthBuffer->storage(depthBits == 16 ? gl::GL_DEPTH_COMPONENT16 : gl::GL_DEPTH_COMPONENT, width, height);
}

glo::Texture* GlBlendAlgorithm::getOutput()
{
    return m_colorTex;
}
