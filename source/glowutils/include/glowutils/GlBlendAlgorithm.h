#pragma once

#include <glowutils/glowutils_api.h>
#include <glowutils/AbstractTransparencyAlgorithm.h>

namespace glow {

class Program;
class FrameBufferObject;
class Texture;
class RenderBufferObject;

}

namespace glowutils {

class Camera;

class GLOWUTILS_API GlBlendAlgorithm : public AbstractTransparencyAlgorithm {

public:
    virtual void initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) override;
    virtual void draw(const DrawFunction& drawFunction, Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual glow::Texture* getOutput() override;

private:
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
    glow::ref_ptr<glow::Texture> m_colorTex;
    glow::ref_ptr<glow::RenderBufferObject> m_depthBuffer;
};

} // namespace glow
