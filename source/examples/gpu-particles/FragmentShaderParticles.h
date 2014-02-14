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
        const std::vector<glm::vec4> & positions
    ,   const std::vector<glm::vec4> & velocities
    ,   const glow::Texture & forces
    ,   const glowutils::Camera & camera);
    virtual ~FragmentShaderParticles();

    virtual void initialize() override;
    virtual void reset() override;

    virtual void step(float elapsed) override;
    virtual void draw(float elapsed) override;

    virtual void resize() override;

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
