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

class ABufferAlgorithm : public AbstractTransparencyAlgorithm {
public:
    virtual void initialize() override;
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual Texture* getOutput() override { return m_colorBuffer.get(); }

private:
    // geometry pass
    ref_ptr<Program> m_program;
    ref_ptr<FrameBufferObject> m_renderFbo;
    ref_ptr<Texture> m_opaqueBuffer;
    ref_ptr<RenderBufferObject> m_depthBuffer;

    // A Buffer
    ref_ptr<Buffer> m_linkedListBuffer;
    ref_ptr<Buffer> m_headBuffer;
    ref_ptr<Buffer> m_counter;

    // post processing pass
    ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    ref_ptr<FrameBufferObject> m_postFbo;
    ref_ptr<Texture> m_colorBuffer;
};

} // namespace glow

