
#include <GL/glew.h>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/FrameBufferObject.h>

#include <glowutils/Camera.h>
#include <glowutils/File.h>
#include <glowutils/ScreenAlignedQuad.h>

#include "FragmentShaderParticles.h"


using namespace glow;
using namespace glm;


FragmentShaderParticles::FragmentShaderParticles(
    const Array<vec4> & positions
,   const Array<vec4> & velocities
,   const Texture & forces
,   const Camera & camera)
: AbstractParticleTechnique(positions, velocities, forces, camera)
{
}

FragmentShaderParticles::~FragmentShaderParticles()
{
}

void FragmentShaderParticles::initialize()
{
    // Create vertex buffers for position and velocity
    m_sourcePositions  = new glow::Buffer(GL_ARRAY_BUFFER);
    m_sourceVelocities = new glow::Buffer(GL_ARRAY_BUFFER);

    // Fill buffers with data
    reset();

    // Create vertex array object
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

    // Bind vertex data to texture objects
    m_sourcePositionsTexture = new glow::Texture();
    m_sourcePositionsTexture->bind(GL_TEXTURE_BUFFER);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_sourcePositions->id());
    m_sourcePositionsTexture->unbind(GL_TEXTURE_BUFFER);

    m_sourceVelocitiesTexture = new glow::Texture();
    m_sourceVelocitiesTexture->bind(GL_TEXTURE_BUFFER);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_sourceVelocities->id());
    m_sourceVelocitiesTexture->unbind(GL_TEXTURE_BUFFER);

    // Create frame buffer object for update
    m_destPositionsTexture = new Texture(GL_TEXTURE_2D);
    m_destPositionsTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_destPositionsTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_destPositionsTexture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_destPositionsTexture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_destPositionsTexture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    m_destPositionsTexture->image2D(0, GL_RGBA32F, 1000, 1000, 0, GL_RGBA, GL_FLOAT, nullptr);

    m_fboParticles = new FrameBufferObject();
    m_fboParticles->attachTexture2D(GL_COLOR_ATTACHMENT0, m_destPositionsTexture);
    m_fboParticles->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });
    m_fboParticles->unbind();

    m_update = new ScreenAlignedQuad(
        createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/test.frag"),
        m_sourcePositionsTexture );
    m_update->setSamplerUniform(0);

    // Create frame buffer object for rendering
    m_fbo = new FrameBufferObject();

    m_colorBuffer = new Texture(GL_TEXTURE_2D);
    m_colorBuffer->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_colorBuffer->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_colorBuffer->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_colorBuffer->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_colorBuffer->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_colorBuffer);
    m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });
    m_fbo->unbind();

    // Create screen aligned quads for clear and rendering
    m_clear = new ScreenAlignedQuad(
        createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/clear.frag") );
    m_quad  = new ScreenAlignedQuad(m_colorBuffer);

    // Create draw program
    m_drawProgram = new Program();
    m_drawProgram->attach(
        createShaderFromFile(GL_VERTEX_SHADER,   "data/gpu-particles/points.vert")
    ,   createShaderFromFile(GL_GEOMETRY_SHADER, "data/gpu-particles/points.geom")
    ,   createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/points.frag"));
}

void FragmentShaderParticles::reset()
{
    // Upload vertex data
    m_sourcePositions ->setData(m_positions,  GL_DYNAMIC_DRAW);
    m_sourceVelocities->setData(m_velocities, GL_DYNAMIC_DRAW);
}

void FragmentShaderParticles::step(const float elapsed)
{
    m_fboParticles->bind();
    m_update->draw();
}

void FragmentShaderParticles::draw(const float elapsed)
{
    // Bind vertex arrays
    m_vao->binding(0)->setBuffer(m_sourcePositions,  0, sizeof(glm::vec4));
    m_vao->binding(1)->setBuffer(m_sourceVelocities, 0, sizeof(glm::vec4));

    // Disable depth buffer
    glDisable(GL_DEPTH_TEST);

    // Activate FBO
    m_fbo->bind();

    // Clear color buffer
    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    m_clear->program()->setUniform("elapsed", elapsed);
    m_clear->draw();

    // Draw particles
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

    // Re-enable depth buffer
    glEnable(GL_DEPTH_TEST);
}

void FragmentShaderParticles::resize()
{
    m_drawProgram->setUniform("aspect", m_camera.aspectRatio());

    m_colorBuffer->image2D(0, GL_RGB16F, m_camera.viewport().x, m_camera.viewport().y, 0, GL_RGB, GL_FLOAT, nullptr);

    m_fbo->bind();

    glClear(GL_COLOR_BUFFER_BIT);

    m_fbo->unbind();
}
