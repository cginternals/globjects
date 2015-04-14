
#include "FragmentShaderParticles.h"

#include <glbinding/gl/gl.h>

#include <globjects/Program.h>
#include <globjects/Buffer.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>

#include <globjects/base/File.h>

#include <common/ScreenAlignedQuad.h>
#include <common/Camera.h>


using namespace gl;
using namespace glm;
using namespace globjects;

FragmentShaderParticles::FragmentShaderParticles(
    const std::vector<vec4> & positions
,   const std::vector<vec4> & velocities
,   const Texture & forces
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

    m_positionsTex.reset(new Texture(GL_TEXTURE_2D));
    m_positionsTex->setParameter(GL_TEXTURE_MIN_FILTER, static_cast<GLint>(GL_NEAREST));
    m_positionsTex->setParameter(GL_TEXTURE_MAG_FILTER, static_cast<GLint>(GL_NEAREST));
    m_positionsTex->setParameter(GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_CLAMP_TO_EDGE));
    m_positionsTex->setParameter(GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_CLAMP_TO_EDGE));
    m_positionsTex->setParameter(GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_CLAMP_TO_EDGE));

    m_velocitiesTex.reset(new Texture(GL_TEXTURE_2D));
    m_velocitiesTex->setParameter(GL_TEXTURE_MIN_FILTER, static_cast<GLint>(GL_NEAREST));
    m_velocitiesTex->setParameter(GL_TEXTURE_MAG_FILTER, static_cast<GLint>(GL_NEAREST));
    m_velocitiesTex->setParameter(GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_CLAMP_TO_EDGE));
    m_velocitiesTex->setParameter(GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_CLAMP_TO_EDGE));
    m_velocitiesTex->setParameter(GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_CLAMP_TO_EDGE));

    reset();

    m_vao.reset(new VertexArray());

    m_updateFbo.reset(new Framebuffer());
    m_updateFbo->bind();
    m_updateFbo->attachTexture(GL_COLOR_ATTACHMENT0, m_positionsTex.get());
    m_updateFbo->attachTexture(GL_COLOR_ATTACHMENT1, m_velocitiesTex.get());
    m_updateFbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 });
    m_updateFbo->unbind();

    m_updateSource.reset(new File("data/gpu-particles/particle.frag"));
    m_updateShader.reset(new Shader(GL_FRAGMENT_SHADER, m_updateSource.get()));

    m_updateQuad.reset(new ScreenAlignedQuad(m_updateShader.get(), m_positionsTex.get()));

    m_verticesUniform.reset(new Uniform<int>("vertices", 0));
    m_velocitiesUniform.reset(new Uniform<int>("velocities", 1));
    m_forcesUniform.reset(new Uniform<int>("forces", 2));

    m_elapsedUniform.reset(new Uniform<float>("elapsed", 0.0f));

    m_updateQuad->program()->attach(
        m_verticesUniform.get(),
        m_velocitiesUniform.get(),
        m_forcesUniform.get(),
        m_elapsedUniform.get()
    );

    m_textureWidthUniform.reset(new Uniform<int>("texWidth", m_workGroupSize.x));

    AbstractParticleTechnique::initialize("data/gpu-particles/points_fragment.vert");

    m_drawProgram->attach(m_textureWidthUniform.get());
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

    m_updateFbo->bind();

    m_positionsTex->bindActive(GL_TEXTURE0);
    m_velocitiesTex->bindActive(GL_TEXTURE1);
    m_forces.bindActive(GL_TEXTURE2);

    m_elapsedUniform->set(elapsed);

    glViewport(0, 0, m_workGroupSize.x, m_workGroupSize.y);
    m_updateQuad->draw();
    glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);

    m_updateFbo->unbind();
}

void FragmentShaderParticles::draw_impl()
{
    m_positionsTex->bindActive(GL_TEXTURE0);
    m_velocitiesTex->bindActive(GL_TEXTURE1);

    m_textureWidthUniform->set(m_workGroupSize.x);

    m_drawProgram->use();

    m_vao->bind();
    m_vao->drawArrays(GL_POINTS, 0, m_numParticles);
    m_vao->unbind();

    m_positionsTex->unbind();
    m_drawProgram->release();
}
