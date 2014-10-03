#pragma once

#include <globjects/base/ref_ptr.h>

#include <globjects/TransformFeedback.h>
#include <globjects/VertexArray.h>
#include <globjects/Program.h>
#include <globjects/Buffer.h>

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

protected:
    virtual void draw_impl() override;

protected:
    globjects::ref_ptr<globjects::TransformFeedback> m_transformFeedback;
    globjects::ref_ptr<globjects::Program> m_transformFeedbackProgram;

    globjects::ref_ptr<globjects::Buffer> m_sourcePositions;
    globjects::ref_ptr<globjects::Buffer> m_sourceVelocities;
    globjects::ref_ptr<globjects::Buffer> m_targetPositions;
    globjects::ref_ptr<globjects::Buffer> m_targetVelocities;

    globjects::ref_ptr<globjects::VertexArray> m_vao;
};
