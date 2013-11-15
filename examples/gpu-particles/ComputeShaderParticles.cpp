
#include <GL/glew.h>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>

#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>

#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/Camera.h>
#include <glowutils/File.h>

#include "ComputeShaderParticles.h"


using namespace glow;
using namespace glm;


ComputeShaderParticles::ComputeShaderParticles(
    const Array<vec4> & positions
,   const Array<vec4> & velocities
,   const Texture & forces
,   const Camera & camera)
: AbstractParticleTechnique(positions, velocities, forces, camera)
, m_quad(nullptr)
, m_clear(nullptr)
{
}

ComputeShaderParticles::~ComputeShaderParticles()
{
    delete m_clear;
    delete m_quad;
}

void ComputeShaderParticles::initialize()
{
    m_computeProgram = new Program();
    m_computeProgram->attach(
        createShaderFromFile(GL_COMPUTE_SHADER, "data/gpu-particles/particle.comp"));

    m_drawProgram = new Program();
    m_drawProgram->attach(
        createShaderFromFile(GL_VERTEX_SHADER, "data/gpu-particles/points.vert")
    ,   createShaderFromFile(GL_GEOMETRY_SHADER, "data/gpu-particles/points.geom")
    ,   createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/points.frag"));
    
    m_drawProgram->use();
    m_drawProgram->bindAttributeLocation(0, "a_vertex");
    m_drawProgram->release();

    m_positionsSSBO = new Buffer(GL_SHADER_STORAGE_BUFFER);
    m_velocitiesSSBO = new Buffer(GL_SHADER_STORAGE_BUFFER);

    reset();

    m_vao = new VertexArrayObject();
    m_vao->bind();

    auto positionsBinding = m_vao->binding(0);
    positionsBinding->setBuffer(m_positionsSSBO, 0, sizeof(vec4));
    positionsBinding->setFormat(4, GL_FLOAT, GL_FALSE, 0);
    m_vao->enable(0);

    auto velocitiesBinding = m_vao->binding(1);
    velocitiesBinding->setBuffer(m_velocitiesSSBO, 0, sizeof(vec4));
    velocitiesBinding->setFormat(4, GL_FLOAT, GL_FALSE, 0);
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

void ComputeShaderParticles::reset()
{
    m_positionsSSBO->setData(m_positions, GL_STATIC_DRAW);
    m_velocitiesSSBO->setData(m_velocities, GL_STATIC_DRAW);
}

void ComputeShaderParticles::step(const float elapsed)
{
    m_positionsSSBO->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    m_velocitiesSSBO->bindBase(GL_SHADER_STORAGE_BUFFER, 1);

    m_forces.bind();
    m_clear->program()->setUniform("elapsed", elapsed);
    m_computeProgram->setUniform("forces", 0);
    m_computeProgram->setUniform("elapsed", elapsed);

    m_computeProgram->use();

    int n = m_numParticles;
    do
    {
        m_computeProgram->setUniform("offset", m_numParticles - n);
        m_computeProgram->dispatchCompute(static_cast<GLuint>(ceil(min(262144.f, static_cast<float>(n)) / 16.f)), 1, 1);
        n -= 262144;
    } while (n > 0);

    m_computeProgram->release();

    m_forces.unbind();

    m_positionsSSBO->unbind();
    m_velocitiesSSBO->unbind();

    //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ComputeShaderParticles::draw()
{
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

    m_quad->draw();

    glEnable(GL_DEPTH_TEST);
}

void ComputeShaderParticles::resize()
{
    m_drawProgram->setUniform("aspect", m_camera.aspectRatio());

    m_color->image2D(0, GL_RGB16F, m_camera.viewport().x, m_camera.viewport().y, 0, GL_RGB, GL_FLOAT, nullptr);
}
