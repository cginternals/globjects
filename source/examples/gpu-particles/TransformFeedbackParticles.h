#pragma once

#include <globjects/base/ref_ptr.h>

#include <globjects/Buffer.h>
#include <globjects/Framebuffer.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <globjects/TransformFeedback.h>
#include <globjects/VertexArray.h>

#include <common/ScreenAlignedQuad.h>

#include "AbstractParticleTechnique.h"

class TransformFeedbackParticles : public AbstractParticleTechnique
{
public:
    TransformFeedbackParticles(
        const std::vector<glm::vec4> & positions
    ,   const std::vector<glm::vec4> & velocities
    ,   const globjects::Texture & forces
    ,   const Camera & camera);
    virtual ~TransformFeedbackParticles();

    virtual void initialize() override;
    virtual void reset() override;

    virtual void step(float elapsed) override;
    virtual void draw(float elapsed) override;

    virtual void resize() override;

protected:
    globjects::ref_ptr<globjects::TransformFeedback> m_transformFeedback;
    globjects::ref_ptr<globjects::Program> m_transformFeedbackProgram;

    globjects::ref_ptr<globjects::Buffer> m_sourcePositions;
    globjects::ref_ptr<globjects::Buffer> m_sourceVelocities;
    globjects::ref_ptr<globjects::Buffer> m_targetPositions;
    globjects::ref_ptr<globjects::Buffer> m_targetVelocities;

    globjects::ref_ptr<globjects::Program> m_drawProgram;

    globjects::ref_ptr<globjects::VertexArray> m_vao;

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<globjects::Texture> m_color;

    globjects::ref_ptr<ScreenAlignedQuad> m_quad;
    globjects::ref_ptr<ScreenAlignedQuad> m_clear;
};
