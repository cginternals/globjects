#include "HybridAlgorithm.h"

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

void HybridAlgorithm::initialize() {
    glow::Shader* vertexShader = glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/transparency/hybrid.vert");

    m_opaqueProgram = new glow::Program;
    m_opaqueProgram->attach(vertexShader);
    m_opaqueProgram->attach(glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/transparency/hybrid_opaque.frag"));

    m_depthBuffer = new RenderBufferObject;
    m_kDepthTab = new Buffer(GL_SHADER_STORAGE_BUFFER);

    m_fbo = new FrameBufferObject;
    m_fbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, m_depthBuffer.get());
}

void HybridAlgorithm::draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) {
}

void HybridAlgorithm::resize(int width, int height) {
}

} // namespace glow
