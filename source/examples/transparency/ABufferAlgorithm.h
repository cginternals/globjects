#pragma once

#include "AbstractTransparencyAlgorithm.h"

#include <globjects/Program.h>
#include <globjects/Framebuffer.h>
#include <globjects/RenderBufferObject.h>
#include <globjects/Texture.h>
#include <globjects/Buffer.h>

#include <globjects-utils/ScreenAlignedQuad.h>

namespace gloutils {

class Camera;

}

/**
    Implements the A-Buffer transparency algorithm.
    Expected shader files:
    abuffer.glsl			- defines functions used in different shaders
    abuffer.frag			- stores the transcluent fragments per pixel in the A-Buffer
    abuffer_post.frag		- combines the opaque and transcluent geometry by sorting and blending the a A-Buffer
*/
class ABufferAlgorithm : public AbstractTransparencyAlgorithm {
public:
    virtual void initialize(const std::string & transparencyShaderFilePath, glo::Shader *vertexShader, glo::Shader *geometryShader) override;
    virtual void draw(const DrawFunction& drawFunction, gloutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual glo::Texture* getOutput() override;

private:
    // geometry pass
    glo::ref_ptr<glo::Program> m_program;
    glo::ref_ptr<glo::Framebuffer> m_renderFbo;
    glo::ref_ptr<glo::Texture> m_opaqueBuffer;
    glo::ref_ptr<glo::RenderBufferObject> m_depthBuffer;

    // A Buffer
    glo::ref_ptr<glo::Buffer> m_linkedListBuffer;
    glo::ref_ptr<glo::Buffer> m_headBuffer;
    glo::ref_ptr<glo::Buffer> m_counter;

    // post processing pass
    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_quad;
    glo::ref_ptr<glo::Framebuffer> m_postFbo;
    glo::ref_ptr<glo::Texture> m_colorBuffer;
};
