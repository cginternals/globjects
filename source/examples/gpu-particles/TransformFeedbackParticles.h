#pragma once

#include <glowbase/ref_ptr.h>

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
    glowbase::ref_ptr<glow::TransformFeedback> m_transformFeedback;
    glowbase::ref_ptr<glow::Program> m_transformFeedbackProgram;

    glowbase::ref_ptr<glow::Buffer> m_sourcePositions;
    glowbase::ref_ptr<glow::Buffer> m_sourceVelocities;
    glowbase::ref_ptr<glow::Buffer> m_targetPositions;
    glowbase::ref_ptr<glow::Buffer> m_targetVelocities;

    glowbase::ref_ptr<glow::Program> m_drawProgram;

    glowbase::ref_ptr<glow::VertexArrayObject> m_vao;

    glowbase::ref_ptr<glow::FrameBufferObject> m_fbo;
    glowbase::ref_ptr<glow::Texture> m_color;

    glowbase::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    glowbase::ref_ptr<glowutils::ScreenAlignedQuad> m_clear;
};
