
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
,   const glowutils::Camera & camera)
: AbstractParticleTechnique(positions, velocities, forces, camera)
{
}

FragmentShaderParticles::~FragmentShaderParticles()
{
}

void FragmentShaderParticles::initialize()
{
    // Create textures to store the particle data
    m_texPositions = new glow::Texture(GL_TEXTURE_2D);
    m_texPositions->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_texPositions->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_texPositions->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_texPositions->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_texPositions->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_texVelocities = new glow::Texture(GL_TEXTURE_2D);
    m_texVelocities->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_texVelocities->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_texVelocities->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_texVelocities->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_texVelocities->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Fill buffers with data
    reset();

    // Create empty vertex array object
    m_vao = new VertexArrayObject();

    // Create frame buffer object for update
    m_fboUpdate = new FrameBufferObject();
    m_fboUpdate->attachTexture2D(GL_COLOR_ATTACHMENT0, m_texPositions);
    m_fboUpdate->attachTexture2D(GL_COLOR_ATTACHMENT1, m_texVelocities);
    m_fboUpdate->setDrawBuffers({GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1});
    m_fboUpdate->unbind();

    // Create screen aligned quad for particle update
    m_quadUpdate = new glowutils::ScreenAlignedQuad(
        glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/particle.frag"),
        m_texPositions );
    m_quadUpdate->program()->setUniform("vertices",   0);
    m_quadUpdate->program()->setUniform("velocities", 1);
    m_quadUpdate->program()->setUniform("forces",     2);

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
    m_clear = new glowutils::ScreenAlignedQuad(
        glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/clear.frag") );
    m_quad  = new glowutils::ScreenAlignedQuad(m_colorBuffer);

    // Create draw program
    m_drawProgram = new Program();
    m_drawProgram->attach(
        glowutils::createShaderFromFile(GL_VERTEX_SHADER,   "data/gpu-particles/points_fragment.vert")
    ,   glowutils::createShaderFromFile(GL_GEOMETRY_SHADER, "data/gpu-particles/points.geom")
    ,   glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/gpu-particles/points.frag"));
}

void FragmentShaderParticles::reset()
{
    // Read positions and velocities into textures
    m_texPositions ->image2D(0, GL_RGBA32F, 1000, 1000, 0, GL_RGBA, GL_FLOAT, m_positions .rawData());
    m_texVelocities->image2D(0, GL_RGBA32F, 1000, 1000, 0, GL_RGBA, GL_FLOAT, m_velocities.rawData());
}

void FragmentShaderParticles::step(const float elapsed)
{
    // Simulate particles via fragment shader
    // Use positions and velocities textures for both input and output at the same time

    m_fboUpdate->bind();
    m_texPositions->bind(GL_TEXTURE0);
    m_texVelocities->bind(GL_TEXTURE1);
    m_forces.bind(GL_TEXTURE2);
    m_quadUpdate->program()->setUniform("elapsed", elapsed);

    glViewport(0, 0, 1000, 1000);
    m_quadUpdate->draw();
    glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);

    m_fboUpdate->unbind();
}

void FragmentShaderParticles::draw(const float elapsed)
{
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
    m_texPositions->bind(GL_TEXTURE0);
    m_drawProgram->setUniform("vertices", 0);
    m_texVelocities->bind(GL_TEXTURE1);
    m_drawProgram->setUniform("velocities", 1);
    m_drawProgram->use();

    m_vao->bind();
    m_vao->drawArrays(GL_POINTS, 0, m_numParticles);
    m_vao->unbind();

    m_texPositions->unbind();

    m_drawProgram->release();

    glDisable(GL_BLEND);

    m_fbo->unbind();

    m_quad->draw();

    // Re-enable depth buffer
    glEnable(GL_DEPTH_TEST);
}

void FragmentShaderParticles::resize()
{
    // Update size of color buffer and ascept ratio

    m_drawProgram->setUniform("aspect", m_camera.aspectRatio());

    m_colorBuffer->image2D(0, GL_RGB16F, m_camera.viewport().x, m_camera.viewport().y, 0, GL_RGB, GL_FLOAT, nullptr);

    m_fbo->bind();
    glClear(GL_COLOR_BUFFER_BIT);
    m_fbo->unbind();
}
