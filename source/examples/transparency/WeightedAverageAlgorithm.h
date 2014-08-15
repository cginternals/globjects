#pragma once

#include "AbstractTransparencyAlgorithm.h"

#include <globjects/Buffer.h>
#include <globjects/FrameBufferObject.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <globjects/RenderBufferObject.h>

#include <globjects-utils/ScreenAlignedQuad.h>

namespace gloutils {

class Camera;

}

/**
    Implements the weighted average transparency algorithm.
    Expected shader files:
        wavg_opaque.frag		- renders the opaque geometry
        wavg_translucent.frag	- renders the transcluent geometry and counts the fragments per pixel
        wavg_post.frag			- combines the opaque and transcluent geometry using weighted average
*/
class WeightedAverageAlgorithm : public AbstractTransparencyAlgorithm
{
public:
    virtual void initialize(const std::string & transparencyShaderFilePath, glo::Shader *vertexShader, glo::Shader *geometryShader) override;
    virtual void draw(const DrawFunction& drawFunction, gloutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual glo::Texture* getOutput() override;

private:
    // geometry pass
    glo::ref_ptr<glo::Program> m_opaqueProgram;
    glo::ref_ptr<glo::Program> m_accumulationProgram;
    glo::ref_ptr<glo::Texture> m_opaqueBuffer;
    glo::ref_ptr<glo::Texture> m_accumulationBuffer;
    glo::ref_ptr<glo::RenderBufferObject> m_depthBuffer;
    glo::ref_ptr<glo::FrameBufferObject> m_renderFbo;
    glo::ref_ptr<glo::Buffer> m_depthComplexityBuffer;

    // post processing pass
    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_quad;
    glo::ref_ptr<glo::FrameBufferObject> m_postFbo;
    glo::ref_ptr<glo::Texture> m_colorBuffer;
};
