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
    ref_ptr<Program> m_opaqueProgram;
    ref_ptr<FrameBufferObject> m_fbo;
    ref_ptr<RenderBufferObject> m_depthBuffer;
    ref_ptr<Texture> m_opaqueBuffer;

    ref_ptr<Program> m_depthKTabProgram;
    ref_ptr<Buffer> m_depthKTab;

    ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    ref_ptr<FrameBufferObject> m_postFbo;
    ref_ptr<Texture> m_colorBuffer;
};

} // namespace glow