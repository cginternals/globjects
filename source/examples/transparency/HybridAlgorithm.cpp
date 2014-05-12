#include "HybridAlgorithm.h"

#include <glbinding/functions.h>

#include <glow/Program.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>
#include <glow/Buffer.h>
#include <glow/NamedString.h>

#include <glowutils/File.h>
#include <glowutils/Camera.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/glowutils.h>

namespace {

const int ABUFFER_SIZE = 4;
const int VISIBILITY_KTAB_SIZE = ABUFFER_SIZE + 1;

}

void HybridAlgorithm::initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) {
    glow::NamedString::create("/transparency/hybrid_definitions", "const int ABUFFER_SIZE = " + std::to_string(ABUFFER_SIZE) + ";");
    glow::NamedString::create("/transparency/hybrid.glsl", new glowutils::File(transparencyShaderFilePath + "hybrid.glsl"));

	m_opaqueProgram = new glow::Program;
    m_opaqueProgram->attach(vertexShader);
	m_opaqueProgram->attach(glowutils::createShaderFromFile(gl::FRAGMENT_SHADER, transparencyShaderFilePath + "hybrid_opaque.frag"));
	if (geometryShader != nullptr) m_opaqueProgram->attach(geometryShader);

	m_depthKTabProgram = new glow::Program;
    m_depthKTabProgram->attach(vertexShader);
	m_depthKTabProgram->attach(glowutils::createShaderFromFile(gl::FRAGMENT_SHADER, transparencyShaderFilePath + "hybrid_depthktab.frag"));
	if (geometryShader != nullptr) m_depthKTabProgram->attach(geometryShader);

	m_visibilityKTabProgram = new glow::Program;
	m_visibilityKTabProgram->attach(glowutils::createShaderFromFile(gl::COMPUTE_SHADER, transparencyShaderFilePath + "hybrid_visibilityktab.comp"));

	m_colorProgram = new glow::Program;
    m_colorProgram->attach(vertexShader);
	m_colorProgram->attach(glowutils::createShaderFromFile(gl::FRAGMENT_SHADER, transparencyShaderFilePath + "hybrid_color.frag"));
	if (geometryShader != nullptr) m_colorProgram->attach(geometryShader);

	m_depthBuffer = new glow::RenderBufferObject;
    m_opaqueBuffer = createColorTex();
    m_coreBuffer = createColorTex();
    m_accumulationBuffer = createColorTex();
    m_colorBuffer = createColorTex();
    m_depthKTab = new glow::Buffer();
    m_visibilityKTab = new glow::Buffer();
    m_depthComplexityBuffer = new glow::Buffer();

	m_prepassFbo = new glow::FrameBufferObject;
    m_prepassFbo->attachTexture2D(gl::COLOR_ATTACHMENT0, m_opaqueBuffer.get());
    m_prepassFbo->attachRenderBuffer(gl::DEPTH_ATTACHMENT, m_depthBuffer.get());

	m_colorFbo = new glow::FrameBufferObject;
    m_colorFbo->attachTexture2D(gl::COLOR_ATTACHMENT0, m_coreBuffer.get());
    m_colorFbo->attachTexture2D(gl::COLOR_ATTACHMENT1, m_accumulationBuffer.get());
    m_colorFbo->attachRenderBuffer(gl::DEPTH_ATTACHMENT, m_depthBuffer.get());
    m_colorFbo->setDrawBuffers({ gl::COLOR_ATTACHMENT0, gl::COLOR_ATTACHMENT1 });

	m_compositionQuad = new glowutils::ScreenAlignedQuad(glowutils::createShaderFromFile(gl::FRAGMENT_SHADER, transparencyShaderFilePath + "hybrid_post.frag"));
    m_compositionFbo = new glow::FrameBufferObject;
    m_compositionFbo->attachTexture2D(gl::COLOR_ATTACHMENT0, m_colorBuffer.get());
    m_compositionFbo->setDrawBuffer(gl::COLOR_ATTACHMENT0);
}

void HybridAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
    m_prepassFbo->bind();

    //
    // render opaque geometry
    //
    m_prepassFbo->setDrawBuffer(gl::COLOR_ATTACHMENT0);
    m_prepassFbo->clear(gl::DEPTH_BUFFER_BIT);
    m_prepassFbo->clearBuffer(gl::COLOR, 0, glm::vec4(1.0f));

    gl::Enable(gl::DEPTH_TEST);

    gl::Disable(gl::BLEND);


    m_opaqueProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_opaqueProgram->setUniform("normalmatrix", camera->normal());
    m_opaqueProgram->use();

    drawFunction(m_opaqueProgram.get());

    //
    // render translucent geometry into depth k-TAB (store depth and alpha for the first (minimum depth) k fragments)
    //
    m_prepassFbo->setDrawBuffer(gl::NONE);
    gl::DepthMask(gl::FALSE_);


    static unsigned int initialDepthKTab = std::numeric_limits<unsigned int>::max();
    m_depthKTab->clearData(gl::R32UI, gl::RED, gl::UNSIGNED_INT, &initialDepthKTab);
    m_depthKTab->bindBase(gl::SHADER_STORAGE_BUFFER, 0);

    m_depthKTabProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_depthKTabProgram->setUniform("normalmatrix", camera->normal());
    m_depthKTabProgram->setUniform("screenSize", glm::ivec2(width, height));
    m_depthKTabProgram->use();

    drawFunction(m_depthKTabProgram.get());

    gl::DepthMask(gl::TRUE_);


    m_prepassFbo->unbind();

    gl::MemoryBarrier(gl::SHADER_STORAGE_BARRIER_BIT);


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
    m_visibilityKTab->clearData(gl::R32F, gl::RED, gl::FLOAT, &initialVisibilityKTab);
    m_visibilityKTab->bindBase(gl::SHADER_STORAGE_BUFFER, 1);

    m_visibilityKTabProgram->setUniform("dimension", width * height);
    m_visibilityKTabProgram->dispatchCompute(width * height / 32 + 1, 1, 1);

    gl::MemoryBarrier(gl::SHADER_STORAGE_BARRIER_BIT);


    //
    // render translucent colors
    //
    m_colorFbo->bind();
    m_colorFbo->clearBuffer(gl::COLOR, 0, glm::vec4(0.0f));
    m_colorFbo->clearBuffer(gl::COLOR, 1, glm::vec4(0.0f));

    gl::Enable(gl::BLEND);

    gl::BlendFunc(gl::ONE, gl::ONE);

    gl::DepthMask(gl::FALSE_);


    static unsigned int initialDepthComplexity = 0;
    m_depthComplexityBuffer->clearData(gl::R32UI, gl::RED, gl::UNSIGNED_INT, &initialDepthComplexity);
    m_depthComplexityBuffer->bindBase(gl::SHADER_STORAGE_BUFFER, 2);

    m_colorProgram->setUniform("viewprojectionmatrix", camera->viewProjection());
    m_colorProgram->setUniform("normalmatrix", camera->normal());
    m_colorProgram->setUniform("screenSize", glm::ivec2(width, height));
    m_colorProgram->use();

    drawFunction(m_colorProgram);

    gl::DepthMask(gl::TRUE_);

    gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

    gl::Disable(gl::BLEND);


    m_colorFbo->unbind();

    //
    // compose core and tail
    //
    m_compositionFbo->bind();
    m_compositionFbo->clear(gl::COLOR_BUFFER_BIT);

    m_opaqueBuffer->bindActive(gl::TEXTURE0);
    m_coreBuffer->bindActive(gl::TEXTURE1);
    m_accumulationBuffer->bindActive(gl::TEXTURE2);

    m_compositionQuad->program()->setUniform("screenSize", glm::ivec2(width, height));
    m_compositionQuad->program()->setUniform("opaqueBuffer", 0);
    m_compositionQuad->program()->setUniform("coreBuffer", 1);
    m_compositionQuad->program()->setUniform("accumulationBuffer", 2);
    m_compositionQuad->draw();

    m_opaqueBuffer->unbindActive(gl::TEXTURE0);
    m_coreBuffer->unbindActive(gl::TEXTURE1);
    m_accumulationBuffer->unbindActive(gl::TEXTURE2);

    m_compositionFbo->unbind();
}

void HybridAlgorithm::resize(int width, int height) {
    m_depthBuffer->storage(gl::DEPTH_COMPONENT, width, height);
    m_opaqueBuffer->image2D(0, gl::RGBA, width, height, 0, gl::RGBA, gl::UNSIGNED_BYTE, nullptr);
    m_colorBuffer->image2D(0, gl::RGBA, width, height, 0, gl::RGBA, gl::UNSIGNED_BYTE, nullptr);
    m_accumulationBuffer->image2D(0, gl::RGBA32F, width, height, 0, gl::RGBA, gl::FLOAT, nullptr);
    m_coreBuffer->image2D(0, gl::RGBA, width, height, 0, gl::RGBA, gl::UNSIGNED_BYTE, nullptr);
    m_depthKTab->setData(static_cast<gl::GLsizei>(width * height * ABUFFER_SIZE * sizeof(unsigned int)), nullptr, gl::DYNAMIC_DRAW);
    m_visibilityKTab->setData(static_cast<gl::GLsizei>(width * height * VISIBILITY_KTAB_SIZE * sizeof(float)), nullptr, gl::DYNAMIC_DRAW);
    m_depthComplexityBuffer->setData(static_cast<gl::GLsizei>(width * height * sizeof(unsigned int)), nullptr, gl::DYNAMIC_DRAW);
}

glow::Texture* HybridAlgorithm::getOutput()
{
    return m_colorBuffer;
}
