#pragma once

#include "AbstractTransparencyAlgorithm.h"

namespace glowutils {
class Camera;
class ScreenAlignedQuad;
}

namespace glow {

class Program;
class FrameBufferObject;
class Texture;
class RenderBufferObject;
class Buffer;

class HybridAlgorithm : public AbstractTransparencyAlgorithm {
public:
    virtual void initialize() override;
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual Texture* getOutput() override { return m_colorBuffer.get(); }

private:
    // shared
    ref_ptr<FrameBufferObject> m_prepassFbo;
    ref_ptr<RenderBufferObject> m_depthBuffer;

    // opaque geometry pass
    ref_ptr<Program> m_opaqueProgram;
    ref_ptr<Texture> m_opaqueBuffer;

    // depth k-TAB pass
    ref_ptr<Program> m_depthKTabProgram;
    ref_ptr<Buffer> m_depthKTab;

    // visibility k-TAB pass
    ref_ptr<Program> m_visibilityKTabProgram;
    ref_ptr<Buffer> m_visibilityKTab;

    // translucent color pass
    ref_ptr<Program> m_colorProgram;
    ref_ptr<FrameBufferObject> m_colorFbo;
    ref_ptr<Texture> m_coreBuffer;
    ref_ptr<Texture> m_accumulationBuffer;
    ref_ptr<Buffer> m_depthComplexityBuffer;

    // composition pass
    ref_ptr<FrameBufferObject> m_compositionFbo;
    ref_ptr<glowutils::ScreenAlignedQuad> m_compositionQuad;
    ref_ptr<Texture> m_colorBuffer;
};

} // namespace glow