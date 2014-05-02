#pragma once

#include "AbstractTransparencyAlgorithm.h"

namespace glow {

class Program;
class FrameBufferObject;
class Texture;
class RenderBufferObject;

}

namespace glowutils {

class Camera;

}

class GlBlendAlgorithm : public AbstractTransparencyAlgorithm {

public:
    virtual void initialize(const std::string & transparencyShaderFilePath, glow::Shader *vertexShader, glow::Shader *geometryShader) override;
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual glow::Texture* getOutput() override;

private:
    glowbase::ref_ptr<glow::Program> m_program;
    glowbase::ref_ptr<glow::FrameBufferObject> m_fbo;
    glowbase::ref_ptr<glow::Texture> m_colorTex;
    glowbase::ref_ptr<glow::RenderBufferObject> m_depthBuffer;
};
