
#include "ComputeShaderParticles.h"

#include <glbinding/gl/gl.h>

#include <globjects/base/AbstractStringSource.h>

#include <globjects/globjects.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/VertexArray.h>
#include <globjects/Shader.h>
#include <globjects/VertexAttributeBinding.h>

#include <globjects/base/File.h>

#include <common/ScreenAlignedQuad.h>
#include <common/Camera.h>
#include <common/StringTemplate.h>


using namespace gl;
using namespace glm;
using namespace globjects;

ComputeShaderParticles::ComputeShaderParticles(
    const std::vector<vec4> & positions
,   const std::vector<vec4> & velocities
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
    static const int max_invocations = getInteger(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS);
    static const ivec3 max_count = ivec3(
        getInteger(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0)
      , getInteger(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1)
      , getInteger(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2));

    const int groups = static_cast<int>(ceil(static_cast<float>(m_numParticles) / static_cast<float>(max_invocations)));

    ivec3 workGroupSize;
    workGroupSize.x = max(groups % max_count.x, 1);
    workGroupSize.y = max(max(groups - workGroupSize.x * max_count.x, 1) % max_count.y, 1);
    workGroupSize.z = max(max((groups - workGroupSize.x * max_count.x) - workGroupSize.y * max_count.y, 1) % max_count.z, 1);

    m_workGroupSize = workGroupSize;

    assert(m_workGroupSize.x * m_workGroupSize.y * m_workGroupSize.z * max_invocations >= m_numParticles);
    assert(m_workGroupSize.x * m_workGroupSize.y * m_workGroupSize.z * max_invocations < m_numParticles + max_invocations);

    m_computeProgram = new Program();
    
    StringTemplate * stringTemplate = new StringTemplate(
        new File("data/gpu-particles/particle.comp"));
    stringTemplate->replace("MAX_INVOCATION", max_invocations);
    stringTemplate->update();

    m_computeProgram->attach(new Shader(GL_COMPUTE_SHADER, stringTemplate));

    m_positionsSSBO = new Buffer();
    m_velocitiesSSBO = new Buffer();

    reset();

    m_vao = new VertexArray();
    m_vao->bind();

    auto positionsBinding = m_vao->binding(0);
    positionsBinding->setAttribute(0);
    positionsBinding->setBuffer(m_positionsSSBO, 0, sizeof(vec4));
    positionsBinding->setFormat(4, GL_FLOAT, GL_FALSE, 0);
    m_vao->enable(0);

    auto velocitiesBinding = m_vao->binding(1);
    velocitiesBinding->setAttribute(1);
    velocitiesBinding->setBuffer(m_velocitiesSSBO, 0, sizeof(vec4));
    velocitiesBinding->setFormat(4, GL_FLOAT, GL_FALSE, 0);
    m_vao->enable(1);

    m_vao->unbind();


    AbstractParticleTechnique::initialize("data/gpu-particles/points.vert");
}

void ComputeShaderParticles::reset()
{
    m_positionsSSBO->setData(m_positions, GL_STATIC_DRAW);
    m_velocitiesSSBO->setData(m_velocities, GL_STATIC_DRAW);

    AbstractParticleTechnique::reset();
}

void ComputeShaderParticles::step(const float elapsed)
{
    m_positionsSSBO->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
    m_velocitiesSSBO->bindBase(GL_SHADER_STORAGE_BUFFER, 1);

    m_forces.bind();
    m_computeProgram->setUniform("forces", 0);
    m_computeProgram->setUniform("elapsed", elapsed);

    m_computeProgram->use();
    
    m_computeProgram->dispatchCompute(m_workGroupSize);
    
    m_computeProgram->release();

    m_forces.unbind();

    m_positionsSSBO->unbind(GL_SHADER_STORAGE_BUFFER, 0);
    m_velocitiesSSBO->unbind(GL_SHADER_STORAGE_BUFFER, 1);
}

void ComputeShaderParticles::draw_impl()
{
    m_drawProgram->use();

    m_vao->bind();
    m_vao->drawArrays(GL_POINTS, 0, m_numParticles);
    m_vao->unbind();

    m_drawProgram->release();
}
