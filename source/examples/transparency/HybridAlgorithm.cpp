#include "HybridAlgorithm.h"

#include <glbinding/gl/gl.h>

#include <globjects/Program.h>
#include <globjects/FrameBufferObject.h>
#include <globjects/Texture.h>
#include <globjects/RenderBufferObject.h>
#include <globjects/Buffer.h>
#include <globjects/NamedString.h>

#include <globjects-base/File.h>
#include <globjects-utils/Camera.h>
#include <globjects-utils/ScreenAlignedQuad.h>
#include <globjects-utils/globjects-utils.h>

namespace {

const int ABUFFER_SIZE = 4;
const int VISIBILITY_KTAB_SIZE = ABUFFER_SIZE + 1;

}

void HybridAlgorithm::initialize(const std::string & transparencyShaderFilePath, glo::Shader *vertexShader, glo::Shader *geometryShader) {
    glo::NamedString::create("/transparency/hybrid_definitions", "const int ABUFFER_SIZE = " + std::to_string(ABUFFER_SIZE) + ";");
    glo::NamedString::create("/transparency/hybrid.glsl", new glo::File(transparencyShaderFilePath + "hybrid.glsl"));

	m_opaqueProgram = new glo::Program;
    m_opaqueProgram->attach(vertexShader);
	m_opaqueProgram->attach(glo::Shader::fromFile(gl::GL_FRAGMENT_SHADER, transparencyShaderFilePath + "hybrid_opaque.frag"));
	if (geometryShader != nullptr) m_opaqueProgram->attach(geometryShader);

	m_depthKTabProgram = new glo::Program;
    m_depthKTabProgram->attach(vertexShader);
	m_depthKTabProgram->attach(glo::Shader::fromFile(gl::GL_FRAGMENT_SHADER, transparencyShaderFilePath + "hybrid_depthktab.frag"));
	if (geometryShader != nullptr) m_depthKTabProgram->attach(geometryShader);

	m_visibilityKTabProgram = new glo::Program;
	m_visibilityKTabProgram->attach(glo::Shader::fromFile(gl::GL_COMPUTE_SHADER, transparencyShaderFilePath + "hybrid_visibilityktab.comp"));

	m_colorProgram = new glo::Program;
    m_colorProgram->attach(vertexShader);
	m_colorProgram->attach(glo::Shader::fromFile(gl::GL_FRAGMENT_SHADER, transparencyShaderFilePath + "hybrid_color.frag"));
	if (geometryShader != nullptr) m_colorProgram->attach(geometryShader);

	m_depthBuffer = new glo::RenderBufferObject;
    m_opaqueBuffer = createColorTex();
    m_coreBuffer = createColorTex();
    m_accumulationBuffer = createColorTex();
    m_colorBuffer = createColorTex();
    m_depthKTab = new glo::Buffer();
    m_visibilityKTab = new glo::Buffer();
    m_depthComplexityBuffer = new glo::Buffer();

	m_prepassFbo = new glo::FrameBufferObject;
    m_prepassFbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_opaqueBuffer.get());
    m_prepassFbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depthBuffer.get());

	m_colorFbo = new glo::FrameBufferObject;
    m_colorFbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_coreBuffer.get());
    m_colorFbo->attachTexture(gl::GL_COLOR_ATTACHMENT1, m_accumulationBuffer.get());
    m_colorFbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depthBuffer.get());
    m_colorFbo->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0, gl::GL_COLOR_ATTACHMENT1 });

	m_compositionQuad = new gloutils::ScreenAlignedQuad(glo::Shader::fromFile(gl::GL_FRAGMENT_SHADER, transparencyShaderFilePath + "hybrid_post.frag"));
    m_compositionFbo = new glo::FrameBufferObject;
    m_compositionFbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_colorBuffer.get());
    m_compositionFbo->setDrawBuffer(gl::GL_COLOR_ATTACHMENT0);
}

void HybridAlgorithm::draw(const DrawFunction& drawFunction, gloutils::Camera* camera, int width, int height) {
    m_prepassFbo->bind();

    //
    // render opaque geometry
    //
    m_prepassFbo->setDrawBuffer(gl::GL_COLOR_ATTACHMENT0);
    m_prepassFbo->clear(gl::GL_DEPTH_BUFFER_BIT);
    m_prepassFbo->clearBuffer(gl::GL_COLOR, 0, glm::vec4(1.0f));

    gl::glEnable(gl::GL_DEPTH_TEST);

    gl::glDisable(gl::GL_BLEND);


    m_opaqueProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_opaqueProgram->setUniform("normalmatrix", camera->normal());
    m_opaqueProgram->use();

    drawFunction(m_opaqueProgram.get());

    //
    // render translucent geometry into depth k-TAB (store depth and alpha for the first (minimum depth) k fragments)
    //
    m_prepassFbo->setDrawBuffer(gl::GL_NONE);
    gl::glDepthMask(gl::GL_FALSE);


    static unsigned int initialDepthKTab = std::numeric_limits<unsigned int>::max();
    m_depthKTab->clearData(gl::GL_R32UI, gl::GL_RED, gl::GL_UNSIGNED_INT, &initialDepthKTab);
    m_depthKTab->bindBase(gl::GL_SHADER_STORAGE_BUFFER, 0);

    m_depthKTabProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_depthKTabProgram->setUniform("normalmatrix", camera->normal());
    m_depthKTabProgram->setUniform("screenSize", glm::ivec2(width, height));
    m_depthKTabProgram->use();

    drawFunction(m_depthKTabProgram.get());

    gl::glDepthMask(gl::GL_TRUE);


    m_prepassFbo->unbind();

    gl::glMemoryBarrier(gl::GL_SHADER_STORAGE_BARRIER_BIT);


    //
    // compute visibility k-TAB: The alpha visibility for each of the first k fragments.
	// The alpha blended color for the k front most fragments of a single pixel is calculated by the following equation:
	//	acc_0 = src_0 (src_i is the i-th k-Tab entry of the pixel)
	//	for i = 1 to k: 
	//		factor = 1.0 - acc_(i-1).a
	//		acc_i = acc_(i-1) + src_i * factor
	//	
    // This pass computes "factor" for each fragment so that in the final path the colors of the k fragments can be combined order independent
    static float initialVisibilityKTab = 0.0f;
    m_visibilityKTab->clearData(gl::GL_R32F, gl::GL_RED, gl::GL_FLOAT, &initialVisibilityKTab);
    m_visibilityKTab->bindBase(gl::GL_SHADER_STORAGE_BUFFER, 1);

    m_visibilityKTabProgram->setUniform("dimension", width * height);
    m_visibilityKTabProgram->dispatchCompute(width * height / 32 + 1, 1, 1);

    gl::glMemoryBarrier(gl::GL_SHADER_STORAGE_BARRIER_BIT);


    //
    // render translucent colors
    //
    m_colorFbo->bind();
    m_colorFbo->clearBuffer(gl::GL_COLOR, 0, glm::vec4(0.0f));
    m_colorFbo->clearBuffer(gl::GL_COLOR, 1, glm::vec4(0.0f));

    gl::glEnable(gl::GL_BLEND);

    gl::glBlendFunc(gl::GL_ONE, gl::GL_ONE);

    gl::glDepthMask(gl::GL_FALSE);


    static unsigned int initialDepthComplexity = 0;
    m_depthComplexityBuffer->clearData(gl::GL_R32UI, gl::GL_RED, gl::GL_UNSIGNED_INT, &initialDepthComplexity);
    m_depthComplexityBuffer->bindBase(gl::GL_SHADER_STORAGE_BUFFER, 2);

    m_colorProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_colorProgram->setUniform("normalmatrix", camera->normal());
    m_colorProgram->setUniform("screenSize", glm::ivec2(width, height));
    m_colorProgram->use();

    drawFunction(m_colorProgram);

    gl::glDepthMask(gl::GL_TRUE);

    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

    gl::glDisable(gl::GL_BLEND);


    m_colorFbo->unbind();

    //
    // compose core and tail
    //
    m_compositionFbo->bind();
    m_compositionFbo->clear(gl::GL_COLOR_BUFFER_BIT);

    m_opaqueBuffer->bindActive(gl::GL_TEXTURE0);
    m_coreBuffer->bindActive(gl::GL_TEXTURE1);
    m_accumulationBuffer->bindActive(gl::GL_TEXTURE2);

    m_compositionQuad->program()->setUniform("screenSize", glm::ivec2(width, height));
    m_compositionQuad->program()->setUniform("opaqueBuffer", 0);
    m_compositionQuad->program()->setUniform("coreBuffer", 1);
    m_compositionQuad->program()->setUniform("accumulationBuffer", 2);
    m_compositionQuad->draw();

    m_opaqueBuffer->unbindActive(gl::GL_TEXTURE0);
    m_coreBuffer->unbindActive(gl::GL_TEXTURE1);
    m_accumulationBuffer->unbindActive(gl::GL_TEXTURE2);

    m_compositionFbo->unbind();
}

void HybridAlgorithm::resize(int width, int height) {
    m_depthBuffer->storage(gl::GL_DEPTH_COMPONENT, width, height);
    m_opaqueBuffer->image2D(0, gl::GL_RGBA, width, height, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
    m_colorBuffer->image2D(0, gl::GL_RGBA, width, height, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
    m_accumulationBuffer->image2D(0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, nullptr);
    m_coreBuffer->image2D(0, gl::GL_RGBA, width, height, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
    m_depthKTab->setData(static_cast<gl::GLsizei>(width * height * ABUFFER_SIZE * sizeof(unsigned int)), nullptr, gl::GL_DYNAMIC_DRAW);
    m_visibilityKTab->setData(static_cast<gl::GLsizei>(width * height * VISIBILITY_KTAB_SIZE * sizeof(float)), nullptr, gl::GL_DYNAMIC_DRAW);
    m_depthComplexityBuffer->setData(static_cast<gl::GLsizei>(width * height * sizeof(unsigned int)), nullptr, gl::GL_DYNAMIC_DRAW);
}

glo::Texture* HybridAlgorithm::getOutput()
{
    return m_colorBuffer;
}
