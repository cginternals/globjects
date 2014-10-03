
#include "FragmentShaderParticles.h"

#include <glbinding/gl/gl.h>

#include <globjects/Program.h>
#include <globjects/VertexArray.h>
#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>

#include <globjects/base/File.h>

#include <common/Camera.h>


using namespace gl;
using namespace glm;

FragmentShaderParticles::FragmentShaderParticles(
    const std::vector<vec4> & positions
,   const std::vector<vec4> & velocities
,   const globjects::Texture & forces
,   const Camera & camera)
: AbstractParticleTechnique(positions, velocities, forces, camera)
, m_positionsFilled(m_positions)
, m_velocitiesFilled(m_velocities)
{
}

FragmentShaderParticles::~FragmentShaderParticles()
{
}

void FragmentShaderParticles::initialize()
{
    // use 2d texture for pseudo compute invocations

    const int size = static_cast<int>(m_positions.size());

    m_workGroupSize.x = static_cast<int>(std::sqrt(static_cast<float>(size)));
    m_workGroupSize.y = m_workGroupSize.x;

    const int remain = size - (m_workGroupSize.x * m_workGroupSize.y);
    m_workGroupSize.y += remain / m_workGroupSize.x + (remain % m_workGroupSize.x == 0 ? 0 : 1);

    // inject some null data to fill texture

    const int nullsize = m_workGroupSize.x * m_workGroupSize.y - size;

    m_positionsFilled.resize(m_workGroupSize.x * m_workGroupSize.y);
    m_velocitiesFilled.resize(m_workGroupSize.x * m_workGroupSize.y);

    for (int i = 0; i < nullsize; ++i)
    {
        m_positionsFilled[size + i] = glm::vec4(0.f, 0.f, 0.f, 0.f);
        m_velocitiesFilled[size + i] = glm::vec4(0.f, 0.f, 0.f, 0.f);
    }

    m_positionsTex = new globjects::Texture(GL_TEXTURE_2D);
    m_positionsTex->setParameter(GL_TEXTURE_MIN_FILTER, static_cast<GLint>(GL_NEAREST));
    m_positionsTex->setParameter(GL_TEXTURE_MAG_FILTER, static_cast<GLint>(GL_NEAREST));
    m_positionsTex->setParameter(GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_CLAMP_TO_EDGE));
    m_positionsTex->setParameter(GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_CLAMP_TO_EDGE));
    m_positionsTex->setParameter(GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_CLAMP_TO_EDGE));

    m_velocitiesTex = new globjects::Texture(GL_TEXTURE_2D);
    m_velocitiesTex->setParameter(GL_TEXTURE_MIN_FILTER, static_cast<GLint>(GL_NEAREST));
    m_velocitiesTex->setParameter(GL_TEXTURE_MAG_FILTER, static_cast<GLint>(GL_NEAREST));
    m_velocitiesTex->setParameter(GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_CLAMP_TO_EDGE));
    m_velocitiesTex->setParameter(GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_CLAMP_TO_EDGE));
    m_velocitiesTex->setParameter(GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_CLAMP_TO_EDGE));

    reset();

    m_vao = new globjects::VertexArray();

    m_updateFbo = new globjects::Framebuffer();
    m_updateFbo->bind(GL_FRAMEBUFFER);
    m_updateFbo->attachTexture(GL_COLOR_ATTACHMENT0, m_positionsTex);
    m_updateFbo->attachTexture(GL_COLOR_ATTACHMENT1, m_velocitiesTex);
    m_updateFbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 });
    m_updateFbo->unbind(GL_FRAMEBUFFER);

    m_updateQuad = new ScreenAlignedQuad(
        globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/particle.frag"), m_positionsTex);
    m_updateQuad->program()->setUniform("vertices",   0);
    m_updateQuad->program()->setUniform("velocities", 1);
    m_updateQuad->program()->setUniform("forces",     2);


    AbstractParticleTechnique::initialize("data/gpu-particles/points_fragment.vert");
}

void FragmentShaderParticles::reset()
{
    m_positionsTex->image2D(0, GL_RGBA32F, m_workGroupSize, 0, GL_RGBA, GL_FLOAT, m_positionsFilled.data());
    m_velocitiesTex->image2D(0, GL_RGBA32F, m_workGroupSize, 0, GL_RGBA, GL_FLOAT, m_velocitiesFilled.data());

    AbstractParticleTechnique::reset();
}

void FragmentShaderParticles::step(const float elapsed)
{
    // Use positions and velocities textures for both input and output at the same time

    m_updateFbo->bind(GL_FRAMEBUFFER);

    m_positionsTex->bindActive(GL_TEXTURE0);
    m_velocitiesTex->bindActive(GL_TEXTURE1);
    m_forces.bindActive(GL_TEXTURE2);

    m_updateQuad->program()->setUniform("elapsed", elapsed);

    glViewport(0, 0, m_workGroupSize.x, m_workGroupSize.y);
    m_updateQuad->draw();
    glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);

    m_updateFbo->unbind(GL_FRAMEBUFFER);
}

void FragmentShaderParticles::draw_impl()
{
    m_positionsTex->bindActive(GL_TEXTURE0);
    m_drawProgram->setUniform("vertices", 0);
    m_velocitiesTex->bindActive(GL_TEXTURE1);
    m_drawProgram->setUniform("velocities", 1);
    m_drawProgram->setUniform("texWidth", m_workGroupSize.x);
    m_drawProgram->use();

    m_vao->bind();
    m_vao->drawArrays(GL_POINTS, 0, m_numParticles);
    m_vao->unbind();

    m_positionsTex->unbind();
    m_drawProgram->release();
}
