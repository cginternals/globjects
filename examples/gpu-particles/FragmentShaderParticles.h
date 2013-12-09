#pragma once


#include "AbstractParticleTechnique.h"


namespace glow
{
    class Program;
    class Texture;
    class VertexArrayObject;
    class FrameBufferObject;
}
namespace glowutils
{
    class ScreenAlignedQuad;
}


class FragmentShaderParticles : public AbstractParticleTechnique
{
public:
    FragmentShaderParticles(
        const glow::Array<glm::vec4> & positions
    ,   const glow::Array<glm::vec4> & velocities
    ,   const glow::Texture & forces
    ,   const glowutils::Camera & camera);
    virtual ~FragmentShaderParticles();

    virtual void initialize();
    virtual void reset();

    virtual void step(float elapsed);
    virtual void draw(float elapsed);

    virtual void resize();

protected:
    // Particle data
    glow::ref_ptr<glow::Texture>                m_texPositions;
    glow::ref_ptr<glow::Texture>                m_texVelocities;
    int                                         m_width;
    int                                         m_height;

    // Update of particles
    glow::ref_ptr<glow::FrameBufferObject>      m_fboUpdate;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quadUpdate;

    // Rendering
    glow::ref_ptr<glow::VertexArrayObject>      m_vao;
    glow::ref_ptr<glow::FrameBufferObject>      m_fbo;
    glow::ref_ptr<glow::Texture>                m_colorBuffer;
    glow::ref_ptr<glow::Program>                m_drawProgram;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_clear;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
};
