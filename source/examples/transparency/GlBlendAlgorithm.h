#pragma once

#include "AbstractTransparencyAlgorithm.h"

#include <globjects/Program.h>
#include <globjects/Framebuffer.h>
#include <globjects/RenderBufferObject.h>
#include <globjects/Texture.h>

namespace gloutils {

class Camera;

}

class GlBlendAlgorithm : public AbstractTransparencyAlgorithm
{

public:
    virtual void initialize(const std::string & transparencyShaderFilePath, glo::Shader * vertexShader, glo::Shader * geometryShader) override;
    virtual void draw(const DrawFunction& drawFunction, gloutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual glo::Texture * getOutput() override;

private:
    glo::ref_ptr<glo::Program> m_program;
    glo::ref_ptr<glo::Framebuffer> m_fbo;
    glo::ref_ptr<glo::Texture> m_colorTex;
    glo::ref_ptr<glo::RenderBufferObject> m_depthBuffer;
};
