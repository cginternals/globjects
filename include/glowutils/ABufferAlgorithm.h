#pragma once

#include <glowutils/glowutils.h>
#include <glowutils/AbstractTransparencyAlgorithm.h>

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

/**
    Implements the A-Buffer transparency algorithm.
    Expected shader files:
    abuffer.glsl			- defines functions used in different shaders
    abuffer.frag			- stores the transcluent fragments per pixel in the A-Buffer
    abuffer_post.frag		- combines the opaque and transcluent geometry by sorting and blending the a A-Buffer
*/
class GLOWUTILS_API ABufferAlgorithm : public AbstractTransparencyAlgorithm {
public:
    virtual void initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) override;
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual glow::Texture* getOutput() override;

private:
    // geometry pass
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glow::FrameBufferObject> m_renderFbo;
    glow::ref_ptr<glow::Texture> m_opaqueBuffer;
    glow::ref_ptr<glow::RenderBufferObject> m_depthBuffer;

    // A Buffer
    glow::ref_ptr<glow::Buffer> m_linkedListBuffer;
    glow::ref_ptr<glow::Buffer> m_headBuffer;
    glow::ref_ptr<glow::Buffer> m_counter;

    // post processing pass
    glow::ref_ptr<ScreenAlignedQuad> m_quad;
    glow::ref_ptr<glow::FrameBufferObject> m_postFbo;
    glow::ref_ptr<glow::Texture> m_colorBuffer;
};

} // namespace glow

