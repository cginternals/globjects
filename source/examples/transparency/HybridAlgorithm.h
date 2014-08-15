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
    Implements the hybrid transparency algorithm.
    Expected shader files:
    hybrid.glsl					- defines functions that are used in different shaders
    hybrid_opaque.frag			- renders the opaque geometry
    hybrid_depthktab.frag		- stores the alpha values of the k fragments next to the camera per pixel ordered by depth
    hybrid_visibilityktab.comp	- computes the visibility of the k fragments stored for each pixel
    hybrid_color.frag			- renders the transcluent geometry
    hybrid_post.frag			- combines the transcluent and opaque geometry
*/
class HybridAlgorithm : public AbstractTransparencyAlgorithm {
public:
    virtual void initialize(const std::string & transparencyShaderFilePath, glo::Shader *vertexShader, glo::Shader *geometryShader) override;
    virtual void draw(const DrawFunction& drawFunction, gloutils::Camera* camera, int width, int height) override;
    virtual void resize(int width, int height) override;
    virtual glo::Texture* getOutput() override;

private:
    // shared
    glo::ref_ptr<glo::FrameBufferObject> m_prepassFbo;
    glo::ref_ptr<glo::RenderBufferObject> m_depthBuffer;

    // opaque geometry pass
    glo::ref_ptr<glo::Program> m_opaqueProgram;
    glo::ref_ptr<glo::Texture> m_opaqueBuffer;

    // depth k-TAB pass
    glo::ref_ptr<glo::Program> m_depthKTabProgram;
    glo::ref_ptr<glo::Buffer> m_depthKTab;

    // visibility k-TAB pass
    glo::ref_ptr<glo::Program> m_visibilityKTabProgram;
    glo::ref_ptr<glo::Buffer> m_visibilityKTab;

    // translucent color pass
    glo::ref_ptr<glo::Program> m_colorProgram;
    glo::ref_ptr<glo::FrameBufferObject> m_colorFbo;
    glo::ref_ptr<glo::Texture> m_coreBuffer;
    glo::ref_ptr<glo::Texture> m_accumulationBuffer;
    glo::ref_ptr<glo::Buffer> m_depthComplexityBuffer;

    // composition pass
    glo::ref_ptr<glo::FrameBufferObject> m_compositionFbo;
    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_compositionQuad;
    glo::ref_ptr<glo::Texture> m_colorBuffer;
};
