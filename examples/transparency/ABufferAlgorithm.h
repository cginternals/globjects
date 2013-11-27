#pragma once

#include "TransparencyAlgorithm.h"

namespace glowutils {
class Camera;
}

namespace glow {

class Program;
class FrameBufferObject;
class Texture;
class RenderBufferObject;
class Buffer;

class ABufferAlgorithm : public TransparencyAlgorithm {
public:
    virtual void initialize() override;
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual Texture* getOutput() override { return m_colorTex.get(); }

private:
    ref_ptr<Program> m_program;
    ref_ptr<FrameBufferObject> m_fbo;
    ref_ptr<Texture> m_colorTex;
    ref_ptr<RenderBufferObject> m_depthBuffer;
    ref_ptr<Buffer> m_linkedListBuffer;
    ref_ptr<Buffer> m_headBuffer;
    ref_ptr<Buffer> m_counter;
};

} // namespace glow

