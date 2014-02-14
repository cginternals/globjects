#pragma once

#include <glow/ref_ptr.h>

#include "AbstractParticleTechnique.h"

namespace glow
{
    class Program;
    class Buffer;
    class TransformFeedback;
    class Texture;
    class FrameBufferObject;
    class VertexArrayObject;
}

namespace glowutils
{
    class ScreenAlignedQuad;
}


class TransformFeedbackParticles : public AbstractParticleTechnique
{
public:
    TransformFeedbackParticles(
        const std::vector<glm::vec4> & positions
    ,   const std::vector<glm::vec4> & velocities
    ,   const glow::Texture & forces
    ,   const glowutils::Camera & camera);
    virtual ~TransformFeedbackParticles();

    virtual void initialize() override;
    virtual void reset() override;

    virtual void step(float elapsed) override;
    virtual void draw(float elapsed) override;

    virtual void resize() override;

protected:
    glow::ref_ptr<glow::TransformFeedback> m_transformFeedback;
    glow::ref_ptr<glow::Program> m_transformFeedbackProgram;

    glow::ref_ptr<glow::Buffer> m_sourcePositions;
    glow::ref_ptr<glow::Buffer> m_sourceVelocities;
    glow::ref_ptr<glow::Buffer> m_targetPositions;
    glow::ref_ptr<glow::Buffer> m_targetVelocities;

    glow::ref_ptr<glow::Program> m_drawProgram;

    glow::ref_ptr<glow::VertexArrayObject> m_vao;

    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
    glow::ref_ptr<glow::Texture> m_color;

    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_clear;
};
