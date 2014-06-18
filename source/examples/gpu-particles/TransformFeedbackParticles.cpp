
#include <glbinding/functions.h>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/TransformFeedback.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>

#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/Camera.h>
#include <glowbase/File.h>
#include <glowutils/glowutils.h>

#include "TransformFeedbackParticles.h"


using namespace glow;
using namespace glm;

TransformFeedbackParticles::TransformFeedbackParticles(
    const std::vector<vec4> & positions
,   const std::vector<vec4> & velocities
,   const Texture & forces
,   const glowutils::Camera & camera)
: AbstractParticleTechnique(positions, velocities, forces, camera)
{
}

TransformFeedbackParticles::~TransformFeedbackParticles()
{
}

void TransformFeedbackParticles::initialize()
{
    m_sourcePositions = new glow::Buffer();
    m_sourceVelocities = new glow::Buffer();
    m_targetPositions = new glow::Buffer();
    m_targetVelocities = new glow::Buffer();

    reset();

    m_transformFeedbackProgram = new glow::Program();
    m_transformFeedbackProgram->attach(glow::Shader::fromFile(gl::GL_VERTEX_SHADER, "data/gpu-particles/transformfeedback.vert"));

    m_transformFeedbackProgram->link();

    m_transformFeedback = new glow::TransformFeedback();
    m_transformFeedback->setVaryings(m_transformFeedbackProgram, std::array<const char*, 2>{ { "out_position", "out_velocity" } }, gl::GL_SEPARATE_ATTRIBS);

    m_drawProgram = new Program();
    m_drawProgram->attach(
        glow::Shader::fromFile(gl::GL_VERTEX_SHADER, "data/gpu-particles/points.vert")
    ,   glow::Shader::fromFile(gl::GL_GEOMETRY_SHADER, "data/gpu-particles/points.geom")
    ,   glow::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/gpu-particles/points.frag"));

    m_vao = new VertexArrayObject();
    m_vao->bind();

    auto positionsBinding = m_vao->binding(0);
    positionsBinding->setAttribute(0);
    positionsBinding->setFormat(4, gl::GL_FLOAT);
    m_vao->enable(0);

    auto velocitiesBinding = m_vao->binding(1);
    velocitiesBinding->setAttribute(1);
    velocitiesBinding->setFormat(4, gl::GL_FLOAT);
    m_vao->enable(1);

    m_vao->unbind();

    // setup fbo

    m_fbo = new FrameBufferObject();

    m_color = new Texture(gl::GL_TEXTURE_2D);
    m_color->setParameter(gl::GL_TEXTURE_MIN_FILTER, static_cast<gl::GLint>(gl::GL_NEAREST));
    m_color->setParameter(gl::GL_TEXTURE_MAG_FILTER, static_cast<gl::GLint>(gl::GL_NEAREST));
    m_color->setParameter(gl::GL_TEXTURE_WRAP_S, static_cast<gl::GLint>(gl::GL_CLAMP_TO_EDGE));
    m_color->setParameter(gl::GL_TEXTURE_WRAP_T, static_cast<gl::GLint>(gl::GL_CLAMP_TO_EDGE));
    m_color->setParameter(gl::GL_TEXTURE_WRAP_R, static_cast<gl::GLint>(gl::GL_CLAMP_TO_EDGE));

    m_fbo->attachTexture2D(gl::GL_COLOR_ATTACHMENT0, m_color);

    m_fbo->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0 });
    m_fbo->unbind();

    m_quad = new glowutils::ScreenAlignedQuad(m_color);
    m_clear = new glowutils::ScreenAlignedQuad(
        glow::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/gpu-particles/clear.frag"));
}

void TransformFeedbackParticles::reset()
{
    m_sourcePositions->setData(m_positions, gl::GL_DYNAMIC_DRAW);
    m_sourceVelocities->setData(m_velocities, gl::GL_DYNAMIC_DRAW);
    m_targetPositions->setData(static_cast<gl::GLsizei>(m_numParticles*sizeof(glm::vec4)), nullptr, gl::GL_DYNAMIC_DRAW);
    m_targetVelocities->setData(static_cast<gl::GLsizei>(m_numParticles*sizeof(glm::vec4)), nullptr, gl::GL_DYNAMIC_DRAW);
}

void TransformFeedbackParticles::step(const float elapsed)
{
    m_vao->bind();

    m_vao->binding(0)->setBuffer(m_sourcePositions, 0, sizeof(glm::vec4));
    m_vao->binding(1)->setBuffer(m_sourceVelocities, 0, sizeof(glm::vec4));

    m_targetPositions->bindBase(gl::GL_TRANSFORM_FEEDBACK_BUFFER, 0);
    m_targetVelocities->bindBase(gl::GL_TRANSFORM_FEEDBACK_BUFFER, 1);

    m_forces.bind();
    m_transformFeedbackProgram->setUniform("forces", 0);
    m_transformFeedbackProgram->setUniform("elapsed", elapsed);

    gl::glEnable(gl::GL_RASTERIZER_DISCARD);
    m_transformFeedbackProgram->use();
    m_transformFeedback->bind();
    m_transformFeedback->begin(gl::GL_POINTS);
    m_vao->drawArrays(gl::GL_POINTS, 0, m_numParticles);
    m_transformFeedback->end();
    m_transformFeedback->unbind();
    m_transformFeedbackProgram->release();
    gl::glDisable(gl::GL_RASTERIZER_DISCARD);

    m_vao->unbind();

    m_forces.unbind();

    std::swap(m_sourcePositions, m_targetPositions);
    std::swap(m_sourceVelocities, m_targetVelocities);
}

void TransformFeedbackParticles::draw(const float elapsed)
{
    m_vao->binding(0)->setBuffer(m_sourcePositions, 0, sizeof(glm::vec4));
    m_vao->binding(1)->setBuffer(m_sourceVelocities, 0, sizeof(glm::vec4));

    gl::glDisable(gl::GL_DEPTH_TEST);

    m_fbo->bind();

    gl::glEnable(gl::GL_BLEND);
    gl::glBlendFunc(gl::GL_ZERO, gl::GL_ONE_MINUS_SRC_COLOR);
    m_clear->program()->setUniform("elapsed", elapsed);
    m_clear->draw();

    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE);

    m_drawProgram->setUniform("viewProjection", m_camera.viewProjection());
    m_drawProgram->use();

    m_vao->bind();
    //m_vao->drawArrays(gl::GL_POINTS, 0, m_numParticles); // don't use this, it is erroneous
    m_transformFeedback->draw(gl::GL_POINTS);
    m_vao->unbind();

    m_drawProgram->release();

    gl::glDisable(gl::GL_BLEND);

    m_fbo->unbind();

    m_quad->draw();

    gl::glEnable(gl::GL_DEPTH_TEST);
}

void TransformFeedbackParticles::resize()
{
    m_drawProgram->setUniform("aspect", m_camera.aspectRatio());

    m_color->image2D(0, gl::GL_RGB16F, m_camera.viewport().x, m_camera.viewport().y, 0, gl::GL_RGB, gl::GL_FLOAT, nullptr);

    m_fbo->bind();

    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    m_fbo->unbind();
}
