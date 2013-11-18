
#pragma once

#include <glow/ref_ptr.h>

#include "AbstractParticleTechnique.h"

namespace glow
{
    class Program;
    class Buffer;
    class TransformFeedback;
    class VertexArrayObject;
}


class TransformFeedbackParticles : public AbstractParticleTechnique
{
public:
    TransformFeedbackParticles(
        const glow::Array<glm::vec4> & postions
    ,   const glow::Array<glm::vec4> & velocities
    ,   const glow::Texture & forces
    ,   const glow::Camera & camera);
    virtual ~TransformFeedbackParticles();

    virtual void initialize();
    virtual void reset();

    virtual void step(float elapsed);
    virtual void draw();

    virtual void resize();

protected:
    // transform feedback

    glow::ref_ptr<glow::Program> m_transformFeedbackProgram;
    glow::ref_ptr<glow::TransformFeedback> m_transformFeedback;
    glow::ref_ptr<glow::Buffer> m_transformFeedbackVertexBuffer1;
    glow::ref_ptr<glow::Buffer> m_transformFeedbackVertexBuffer2;
    glow::ref_ptr<glow::VertexArrayObject> m_transformFeedbackVAO;
};
