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
    Implements the hybrid transparency algorithm.
    Expected shader files:
    hybrid.glsl					- defines functions that are used in different shaders
    hybrid_opaque.frag			- renders the opaque geometry
    hybrid_depthktab.frag		- stores the alpha values of the k fragments next to the camera per pixel ordered by depth
    hybrid_visibilityktab.comp	- computes the visibility of the k fragments stored for each pixel
    hybrid_color.frag			- renders the transcluent geometry
    hybrid_post.frag			- combines the transcluent and opaque geometry
*/
class HybridAlgorithm : public AbstractTransparencyAlgorithm {
public:
    virtual void initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) override;
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual glow::Texture* getOutput() override;

private:
    // shared
    glow::ref_ptr<glow::FrameBufferObject> m_prepassFbo;
    glow::ref_ptr<glow::RenderBufferObject> m_depthBuffer;

    // opaque geometry pass
    glow::ref_ptr<glow::Program> m_opaqueProgram;
    glow::ref_ptr<glow::Texture> m_opaqueBuffer;

    // depth k-TAB pass
    glow::ref_ptr<glow::Program> m_depthKTabProgram;
    glow::ref_ptr<glow::Buffer> m_depthKTab;

    // visibility k-TAB pass
    glow::ref_ptr<glow::Program> m_visibilityKTabProgram;
    glow::ref_ptr<glow::Buffer> m_visibilityKTab;

    // translucent color pass
    glow::ref_ptr<glow::Program> m_colorProgram;
    glow::ref_ptr<glow::FrameBufferObject> m_colorFbo;
    glow::ref_ptr<glow::Texture> m_coreBuffer;
    glow::ref_ptr<glow::Texture> m_accumulationBuffer;
    glow::ref_ptr<glow::Buffer> m_depthComplexityBuffer;

    // composition pass
    glow::ref_ptr<glow::FrameBufferObject> m_compositionFbo;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_compositionQuad;
    glow::ref_ptr<glow::Texture> m_colorBuffer;
};
