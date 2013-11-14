
#include <GL/glew.h>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>

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

    m_positionsSSBO = new Buffer(GL_SHADER_STORAGE_BUFFER);
    m_positionsSSBO->setData(m_positions, GL_STATIC_DRAW);
}

void ComputeShaderParticles::reset()
{

}

void ComputeShaderParticles::step(const float elapsed)
{
    //m_vertices->bindBase(GL_SHADER_STORAGE_BUFFER, 0);

    //m_computeProgram->setUniform("delta", delta);

    //m_computeProgram->use();
    //m_computeProgram->dispatchCompute(512 / 16, 1, 1);
    //m_computeProgram->release();

    //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    //m_vertices->unbind();
}

void ComputeShaderParticles::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    m_drawProgram->use();
    m_drawProgram->setUniform("viewProjection", m_camera.viewProjection());

    //m_vao->binding(0)->setBuffer(m_vertices, 0, sizeof(vec4));
    //m_vao->bind();

    //m_vao->drawArrays(GL_POINTS, 0, m_numParticles);

    //m_vao->unbind();

    m_drawProgram->release();

    glDisable(GL_BLEND);
}

void ComputeShaderParticles::resize()
{
    m_drawProgram->setUniform("aspect", m_camera.aspectRatio());
}
