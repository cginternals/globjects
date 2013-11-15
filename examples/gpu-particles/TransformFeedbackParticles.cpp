
#include <GL/glew.h>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/TransformFeedback.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>

#include <glowutils/Camera.h>
#include <glowutils/File.h>

#include "TransformFeedbackParticles.h"


using namespace glow;
using namespace glm;


TransformFeedbackParticles::TransformFeedbackParticles(
    const Array<vec4> & positions
,   const Array<vec4> & velocities
,   const Texture & forces
,   const Camera & camera)
: AbstractParticleTechnique(positions, velocities, forces, camera)
{
}

TransformFeedbackParticles::~TransformFeedbackParticles()
{
}

void TransformFeedbackParticles::initialize()
{
    //m_transformFeedbackProgram = new glow::Program();
    //m_transformFeedbackProgram->attach(glow::createShaderFromFile(GL_VERTEX_SHADER, "data/gpu-particles/transformfeedback.vert"));
    //m_transformFeedbackProgram->setUniform("delta", 0.0f);
    //m_transformFeedbackVertexBuffer1 = new glow::Buffer(GL_ARRAY_BUFFER);
    //m_transformFeedbackVertexBuffer1->setData(glow::Array<glm::vec4>(m_positions));
    //m_transformFeedbackVertexBuffer2 = new glow::Buffer(GL_ARRAY_BUFFER);
    //m_transformFeedbackVertexBuffer2->setData(glow::Array<glm::vec4>(m_positions));

    //m_transformFeedback = new glow::TransformFeedback();
    //m_transformFeedback->setVaryings(m_transformFeedbackProgram, glow::Array<const char*>{ "out_position" }, GL_INTERLEAVED_ATTRIBS);

    //m_transformFeedbackVAO = new glow::VertexArrayObject();
    //m_transformFeedbackVAO->binding(0)->setAttribute(m_transformFeedbackProgram->getAttributeLocation("in_position"));
    //m_transformFeedbackVAO->binding(0)->setFormat(4, GL_FLOAT);
    //m_transformFeedbackVAO->enable(m_transformFeedbackProgram->getAttributeLocation("in_position"));
}

void TransformFeedbackParticles::reset()
{

}

void TransformFeedbackParticles::step(const float elapsed)
{
    //m_transformFeedbackVAO->bind();

    //m_transformFeedbackProgram->setUniform("delta", elapsed);

    //m_transformFeedbackVAO->binding(0)->setBuffer(m_transformFeedbackVertexBuffer1, 0, sizeof(glm::vec4));

    //m_transformFeedbackVertexBuffer2->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

    //glEnable(GL_RASTERIZER_DISCARD);
    //m_transformFeedbackProgram->use();
    //m_transformFeedback->bind();
    //m_transformFeedback->begin(GL_POINTS);
    //m_transformFeedbackVAO->drawArrays(GL_POINTS, 0, m_positions.size());
    //m_transformFeedback->end();
    //m_transformFeedback->unbind();
    //glDisable(GL_RASTERIZER_DISCARD);

    //m_transformFeedbackVAO->unbind();

    //std::swap(m_transformFeedbackVertexBuffer1, m_transformFeedbackVertexBuffer2);
}

void TransformFeedbackParticles::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);

    //m_transformFeedbackVAO->binding(0)->setBuffer(m_transformFeedbackVertexBuffer1, 0, sizeof(vec4));

    //m_drawProgram->use();
    //m_drawProgram->setUniform("viewProjection", m_camera.viewProjection());
    //m_vao->bind();
    ////m_transformFeedback->draw(GL_POINTS);

    //m_vao->drawArrays(GL_POINTS, 0, m_numParticles);

    //m_vao->unbind();
    //m_renderProgram->release();

    glDisable(GL_BLEND);
}

void TransformFeedbackParticles::resize()
{
    //m_drawProgram->setUniform("aspect", m_camera.aspectRatio());
}
