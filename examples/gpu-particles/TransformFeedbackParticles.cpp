
#include <GL/glew.h>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/TransformFeedback.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/FrameBufferObject.h>

#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/Camera.h>
#include <glowutils/File.h>

#include "TransformFeedbackParticles.h"


using namespace glow;
using namespace glm;

namespace
{
    struct Particle
    {
        vec4 position;
        vec4 velocity;
    };
}

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
    m_transformFeedbackBuffer1 = new glow::Buffer(GL_ARRAY_BUFFER);
    m_transformFeedbackBuffer2 = new glow::Buffer(GL_ARRAY_BUFFER);

    reset();

    m_transformFeedbackProgram = new glow::Program();
    m_transformFeedbackProgram->attach(glow::createShaderFromFile(GL_VERTEX_SHADER, "data/gpu-particles/transformfeedback.vert"));

    m_transformFeedbackProgram->link();

    m_transformFeedback = new glow::TransformFeedback();
    m_transformFeedback->setVaryings(m_transformFeedbackProgram, glow::Array<const char*>{ "out_position", "out_velocity" }, GL_INTERLEAVED_ATTRIBS);

    m_transformFeedbackVAO = new glow::VertexArrayObject();

    m_transformFeedbackVAO->binding(0)->setAttribute(0);
    m_transformFeedbackVAO->binding(0)->setFormat(4, GL_FLOAT);
    m_transformFeedbackVAO->enable(0);

    m_transformFeedbackVAO->binding(1)->setAttribute(1);
    m_transformFeedbackVAO->binding(1)->setFormat(4, GL_FLOAT);
    m_transformFeedbackVAO->enable(1);

    m_drawProgram = new Program();
    m_drawProgram->attach(
        createShaderFromFile(GL_VERTEX_SHADER, "data/gpu-particles/points.vert")
    ,   createShaderFromFile(GL_GEOMETRY_SHADER, "data/gpu-particles/points.geom")
    ,   createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/points.frag"));

    m_vao = new VertexArrayObject();
    m_vao->bind();

    auto positionsBinding = m_vao->binding(0);
    positionsBinding->setAttribute(0);
    positionsBinding->setFormat(4, GL_FLOAT);
    m_vao->enable(0);

    auto velocitiesBinding = m_vao->binding(1);
    velocitiesBinding->setAttribute(1);
    velocitiesBinding->setFormat(4, GL_FLOAT);
    m_vao->enable(1);

    m_vao->unbind();

    // setup fbo

    m_fbo = new FrameBufferObject();

    m_color = new Texture(GL_TEXTURE_2D);
    m_color->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_color->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_color->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_color->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_color->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_color);

    m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });
    m_fbo->unbind();

    m_quad = new ScreenAlignedQuad(m_color);
    m_clear = new ScreenAlignedQuad(
        createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/clear.frag"));
}

void TransformFeedbackParticles::reset()
{
    m_transformFeedbackBuffer1->setData(m_numParticles * sizeof(Particle), nullptr, GL_STATIC_DRAW);
    Particle* particles = static_cast<Particle*>(m_transformFeedbackBuffer1->map(GL_WRITE_ONLY));

    for (int i=0; i < m_numParticles; ++i)
    {
        particles[i].position = m_positions[i];
        particles[i].velocity = m_velocities[i];
    }

    m_transformFeedbackBuffer1->unmap();
}

void TransformFeedbackParticles::step(const float elapsed)
{
    return;

    m_transformFeedbackVAO->bind();

    m_transformFeedbackVAO->binding(0)->setBuffer(m_transformFeedbackBuffer1, offsetof(Particle, position), sizeof(Particle));
    m_transformFeedbackVAO->binding(1)->setBuffer(m_transformFeedbackBuffer1, offsetof(Particle, velocity), sizeof(Particle));

    m_transformFeedbackBuffer2->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

    m_forces.bind();
    m_clear->program()->setUniform("elapsed", elapsed);
    m_transformFeedbackProgram->setUniform("forces", 0);
    m_transformFeedbackProgram->setUniform("elapsed", elapsed);

    glEnable(GL_RASTERIZER_DISCARD);
    m_transformFeedbackProgram->use();
    m_transformFeedback->bind();
    m_transformFeedback->begin(GL_POINTS);
    m_transformFeedbackVAO->drawArrays(GL_POINTS, 0, m_numParticles);
    m_transformFeedback->end();
    m_transformFeedback->unbind();
    glDisable(GL_RASTERIZER_DISCARD);

    m_transformFeedbackVAO->unbind();

    std::swap(m_transformFeedbackBuffer1, m_transformFeedbackBuffer2);
}

void TransformFeedbackParticles::draw()
{
    m_vao->binding(0)->setBuffer(m_transformFeedbackBuffer1, offsetof(Particle, position), sizeof(Particle));
    m_vao->binding(1)->setBuffer(m_transformFeedbackBuffer1, offsetof(Particle, velocity), sizeof(Particle));

    glDisable(GL_DEPTH_TEST);

    m_fbo->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    m_clear->draw();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    m_drawProgram->setUniform("viewProjection", m_camera.viewProjection());
    m_drawProgram->use();

    m_vao->bind();
    m_vao->drawArrays(GL_POINTS, 0, m_numParticles);
    m_vao->unbind();

    m_drawProgram->release();

    glDisable(GL_BLEND);

    m_fbo->unbind();

    //glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);

    m_quad->draw();

    m_vao->disable(0);
    m_vao->disable(1);

    glEnable(GL_DEPTH_TEST);
}

void TransformFeedbackParticles::resize()
{
    m_drawProgram->setUniform("aspect", m_camera.aspectRatio());

    m_color->image2D(0, GL_RGB16F_ARB, m_camera.viewport().x, m_camera.viewport().y, 0, GL_RGB, GL_FLOAT, nullptr);

    m_fbo->bind();

    glClear(GL_COLOR_BUFFER_BIT);

    m_fbo->unbind();
}
