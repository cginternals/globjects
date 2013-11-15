
#include <GL/glew.h>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>


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
{
}

ComputeShaderParticles::~ComputeShaderParticles()
{
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

    auto vertexBinding = m_vao->binding(0);
    vertexBinding->setAttribute(0);
    vertexBinding->setBuffer(m_positionsSSBO, 0, sizeof(vec4));
    vertexBinding->setFormat(4, GL_FLOAT, GL_FALSE, 0);
    m_vao->enable(0);

    m_vao->unbind();
}

void ComputeShaderParticles::reset()
{
    m_positionsSSBO->setData(m_positions, GL_STATIC_DRAW);
    m_velocitiesSSBO->setData(m_velocities, GL_STATIC_DRAW);
}

void ComputeShaderParticles::step(const float elapsed)
{
    m_positionsSSBO->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    //m_velocitiesSSBO->bindBase(GL_SHADER_STORAGE_BUFFER, 1);

    //m_computeProgram->setUniform("delta", elapsed);
    m_computeProgram->use();
    m_computeProgram->dispatchCompute(m_numParticles / 16, 1, 1);
    m_computeProgram->release();

    //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ComputeShaderParticles::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    m_drawProgram->setUniform("viewProjection", m_camera.viewProjection());
    m_drawProgram->use();

    m_vao->bind();
    m_vao->drawArrays(GL_POINTS, 0, m_numParticles);
    m_vao->unbind();

    m_drawProgram->release();

    glDisable(GL_BLEND);
}

void ComputeShaderParticles::resize()
{
    m_drawProgram->setUniform("aspect", m_camera.aspectRatio());
}
