
#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/AbstractStringSource.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>

#include <glow/FrameBufferObject.h>
#include <glow/Texture.h>
#include <glow/RenderBufferObject.h>

#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/Camera.h>
#include <glowutils/File.h>
#include <glowutils/StringTemplate.h>
#include <glowutils/glowutils.h>

#include "ComputeShaderParticles.h"


using namespace glow;
using namespace glm;


ComputeShaderParticles::ComputeShaderParticles(
    const std::vector<vec4> & positions
,   const std::vector<vec4> & velocities
,   const Texture & forces
,   const glowutils::Camera & camera)
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
    
    glowutils::StringTemplate * stringTemplate = new glowutils::StringTemplate(
        new glowutils::File("data/gpu-particles/particle.comp"));
    stringTemplate->replace("MAX_INVOCATION", max_invocations);
    stringTemplate->update();

    m_computeProgram->attach(new Shader(GL_COMPUTE_SHADER, stringTemplate));

    m_drawProgram = new Program();
    m_drawProgram->attach(
        glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/gpu-particles/points.vert")
        , glowutils::createShaderFromFile(GL_GEOMETRY_SHADER, "data/gpu-particles/points.geom")
        , glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/points.frag"));

    m_positionsSSBO = new Buffer();
    m_velocitiesSSBO = new Buffer();

    reset();

    m_vao = new VertexArrayObject();
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

    m_quad = new glowutils::ScreenAlignedQuad(m_color);
    m_clear = new glowutils::ScreenAlignedQuad(
        glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/clear.frag"));
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
    m_computeProgram->setUniform("forces", 0);
    m_computeProgram->setUniform("elapsed", elapsed);

    m_computeProgram->use();
    
    m_computeProgram->dispatchCompute(m_workGroupSize);
    
    m_computeProgram->release();

    m_forces.unbind();

    m_positionsSSBO->unbind(GL_SHADER_STORAGE_BUFFER, 0);
    m_velocitiesSSBO->unbind(GL_SHADER_STORAGE_BUFFER, 1);
}

void ComputeShaderParticles::draw(const float elapsed)
{
    glDisable(GL_DEPTH_TEST);

    m_fbo->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    m_clear->program()->setUniform("elapsed", elapsed);
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

    m_fbo->bind();

    glClear(GL_COLOR_BUFFER_BIT);

    m_fbo->unbind();
}
