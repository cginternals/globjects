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
    Implements the weighted average transparency algorithm.
    Expected shader files:
        wavg_opaque.frag		- renders the opaque geometry
        wavg_translucent.frag	- renders the transcluent geometry and counts the fragments per pixel
        wavg_post.frag			- combines the opaque and transcluent geometry using weighted average
*/
class WeightedAverageAlgorithm : public AbstractTransparencyAlgorithm
{
public:
    virtual void initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) override;
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual glow::Texture* getOutput() override;

private:
    // geometry pass
    glowbase::ref_ptr<glow::Program> m_opaqueProgram;
    glowbase::ref_ptr<glow::Program> m_accumulationProgram;
    glowbase::ref_ptr<glow::Texture> m_opaqueBuffer;
    glowbase::ref_ptr<glow::Texture> m_accumulationBuffer;
    glowbase::ref_ptr<glow::RenderBufferObject> m_depthBuffer;
    glowbase::ref_ptr<glow::FrameBufferObject> m_renderFbo;
    glowbase::ref_ptr<glow::Buffer> m_depthComplexityBuffer;

    // post processing pass
    glowbase::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    glowbase::ref_ptr<glow::FrameBufferObject> m_postFbo;
    glowbase::ref_ptr<glow::Texture> m_colorBuffer;
};
