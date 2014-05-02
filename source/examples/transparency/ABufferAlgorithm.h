#pragma once

#include "AbstractTransparencyAlgorithm.h"

namespace glow {

class Program;
class FrameBufferObject;
class Texture;
class RenderBufferObject;
class Buffer;

}

namespace glowutils {

class Camera;
class ScreenAlignedQuad;

}

/**
    Implements the A-Buffer transparency algorithm.
    Expected shader files:
    abuffer.glsl			- defines functions used in different shaders
    abuffer.frag			- stores the transcluent fragments per pixel in the A-Buffer
    abuffer_post.frag		- combines the opaque and transcluent geometry by sorting and blending the a A-Buffer
*/
class ABufferAlgorithm : public AbstractTransparencyAlgorithm {
public:
    virtual void initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) override;
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual glow::Texture* getOutput() override;

private:
    // geometry pass
    glowbase::ref_ptr<glow::Program> m_program;
    glowbase::ref_ptr<glow::FrameBufferObject> m_renderFbo;
    glowbase::ref_ptr<glow::Texture> m_opaqueBuffer;
    glowbase::ref_ptr<glow::RenderBufferObject> m_depthBuffer;

    // A Buffer
    glowbase::ref_ptr<glow::Buffer> m_linkedListBuffer;
    glowbase::ref_ptr<glow::Buffer> m_headBuffer;
    glowbase::ref_ptr<glow::Buffer> m_counter;

    // post processing pass
    glowbase::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    glowbase::ref_ptr<glow::FrameBufferObject> m_postFbo;
    glowbase::ref_ptr<glow::Texture> m_colorBuffer;
};
