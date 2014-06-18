#include "WeightedAverageAlgorithm.h"

#include <glow/Program.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>
#include <glow/Buffer.h>

#include <glowbase/File.h>
#include <glowutils/Camera.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/glowutils.h>

void WeightedAverageAlgorithm::initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) {

    m_opaqueProgram = new glow::Program;
	m_opaqueProgram->attach(glow::Shader::fromFile(gl::GL_FRAGMENT_SHADER, transparencyShaderFilePath + "wavg_opaque.frag"));
    m_opaqueProgram->attach(vertexShader);
	if (geometryShader != nullptr) m_opaqueProgram->attach(geometryShader);

    m_accumulationProgram = new glow::Program;
	m_accumulationProgram->attach(glow::Shader::fromFile(gl::GL_FRAGMENT_SHADER, transparencyShaderFilePath + "wavg_translucent.frag"));
    m_accumulationProgram->attach(vertexShader);
	if (geometryShader != nullptr) m_accumulationProgram->attach(geometryShader);

    m_opaqueBuffer = createColorTex();
    m_accumulationBuffer = createColorTex();
    m_depthBuffer = new glow::RenderBufferObject();

    m_depthComplexityBuffer = new glow::Buffer();

    m_renderFbo = new glow::FrameBufferObject();
    m_renderFbo->attachTexture2D(gl::GL_COLOR_ATTACHMENT0, m_opaqueBuffer);
    m_renderFbo->attachTexture2D(gl::GL_COLOR_ATTACHMENT1, m_accumulationBuffer);
    m_renderFbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depthBuffer);
    m_renderFbo->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0, gl::GL_COLOR_ATTACHMENT1 });

	m_quad = new glowutils::ScreenAlignedQuad(glow::Shader::fromFile(gl::GL_FRAGMENT_SHADER, transparencyShaderFilePath + "wavg_post.frag"));

    m_colorBuffer = createColorTex();
    m_postFbo = new glow::FrameBufferObject;
    m_postFbo->attachTexture2D(gl::GL_COLOR_ATTACHMENT0, m_colorBuffer);
    m_postFbo->setDrawBuffer(gl::GL_COLOR_ATTACHMENT0);
}

void WeightedAverageAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
    static float clearDepth = 1.0f;
    m_renderFbo->bind();
    m_renderFbo->clearBuffer(gl::GL_COLOR, 0, glm::vec4(1.0f)); // clear opaque buffer with background
    m_renderFbo->clearBuffer(gl::GL_COLOR, 1, glm::vec4(0.0f)); // clear accumulation buffer with 0
    m_renderFbo->clearBufferfv(gl::GL_DEPTH, 0, &clearDepth);

    // reset depth complexity
    static unsigned int initialDepthComplexity = 0;
    m_depthComplexityBuffer->clearData(gl::GL_R32UI, gl::GL_RED, gl::GL_UNSIGNED_INT, &initialDepthComplexity);
    m_depthComplexityBuffer->bindBase(gl::GL_SHADER_STORAGE_BUFFER, 0);

    gl::glEnable(gl::GL_DEPTH_TEST);

    gl::glDepthMask(gl::GL_TRUE);

    gl::glDisable(gl::GL_BLEND);


    m_opaqueProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_opaqueProgram->setUniform("normalmatrix", camera->normal());
    m_opaqueProgram->use();

    drawFunction(m_opaqueProgram.get());

    gl::glDepthMask(gl::GL_FALSE);

    gl::glEnable(gl::GL_BLEND);

    gl::glBlendFunc(gl::GL_ONE, gl::GL_ONE);


    m_accumulationProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_accumulationProgram->setUniform("normalmatrix", camera->normal());
    m_accumulationProgram->setUniform("screenSize", glm::ivec2(width, height));
    m_accumulationProgram->use();

    drawFunction(m_accumulationProgram.get());

    m_renderFbo->unbind();

    m_postFbo->bind();
    m_postFbo->clear(gl::GL_COLOR_BUFFER_BIT);

    gl::glDepthMask(gl::GL_TRUE);

    gl::glDisable(gl::GL_BLEND);

    gl::glDisable(gl::GL_DEPTH_TEST);


    m_opaqueBuffer->bindActive(gl::GL_TEXTURE0);
    m_accumulationBuffer->bindActive(gl::GL_TEXTURE1);

    m_quad->program()->setUniform("opaqueBuffer", 0);
    m_quad->program()->setUniform("accumulationBuffer", 1);
    m_quad->program()->setUniform("screenSize", glm::ivec2(width, height));

    m_quad->draw();

    m_opaqueBuffer->unbindActive(gl::GL_TEXTURE0);
    m_accumulationBuffer->unbindActive(gl::GL_TEXTURE1);

    gl::glEnable(gl::GL_DEPTH_TEST);


    m_postFbo->unbind();
}

void WeightedAverageAlgorithm::resize(int width, int height) {
    m_opaqueBuffer->image2D(0, gl::GL_RGBA, width, height, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
    m_accumulationBuffer->image2D(0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
    m_colorBuffer->image2D(0, gl::GL_RGBA, width, height, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
    m_depthBuffer->storage(gl::GL_DEPTH_COMPONENT, width, height);
    m_depthComplexityBuffer->setData(static_cast<gl::GLint>(width * height * sizeof(unsigned int)), nullptr, gl::GL_DYNAMIC_DRAW);
}

glow::Texture* WeightedAverageAlgorithm::getOutput()
{
    return m_colorBuffer;
}
