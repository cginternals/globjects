#pragma once

#include <functional>

#include <glow/ref_ptr.h>

namespace glowutils {
class Camera;
}

namespace glow {

class Program;
class FrameBufferObject;
class Texture;
class RenderBufferObject;
class Buffer;

class TransparencyAlgorithm {
public:
    typedef std::function<void(Program*)> DrawFunction;

    virtual void initialize() {}
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) = 0;
    virtual void resize(int width, int height) {}
    virtual Texture* getOutput() = 0;
};

class GlBlendAlgorithm : public TransparencyAlgorithm {
public:
    virtual void initialize() override;
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual Texture* getOutput() override { return colorTex.get(); }

private:
    ref_ptr<Program> program;
    ref_ptr<FrameBufferObject> fbo;
    ref_ptr<Texture> colorTex;
    ref_ptr<RenderBufferObject> depth;
};

class ABufferAlgorithm : public TransparencyAlgorithm {
public:
    virtual void initialize() override;
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual Texture* getOutput() override;

private:
    ref_ptr<Program> program;
    ref_ptr<FrameBufferObject> fbo;
    ref_ptr<Texture> colorTex;
    ref_ptr<RenderBufferObject> depth;
    ref_ptr<Buffer> linkedListBuffer;
    ref_ptr<Buffer> headBuffer;
    ref_ptr<Buffer> counter;
};

} // namespace glow
