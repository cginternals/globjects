#pragma once


#include "AbstractParticleTechnique.h"


namespace globjects
{
    class Program;
    class Texture;
    class VertexArray;
    class Framebuffer;
}

class ScreenAlignedQuad;


class FragmentShaderParticles : public AbstractParticleTechnique
{
public:
    FragmentShaderParticles(
        const std::vector<glm::vec4> & positions
    ,   const std::vector<glm::vec4> & velocities
    ,   const globjects::Texture & forces
    ,   const Camera & camera);
    virtual ~FragmentShaderParticles();

    virtual void initialize() override;
    virtual void reset() override;

    virtual void step(float elapsed) override;
    virtual void draw(float elapsed) override;

    virtual void resize() override;

protected:
    // Particle data
    globjects::ref_ptr<globjects::Texture>                m_texPositions;
    globjects::ref_ptr<globjects::Texture>                m_texVelocities;
    int                                         m_width;
    int                                         m_height;

    // Update of particles
    globjects::ref_ptr<globjects::Framebuffer>      m_fboUpdate;
    globjects::ref_ptr<ScreenAlignedQuad> m_quadUpdate;

    // Rendering
    globjects::ref_ptr<globjects::VertexArray>      m_vao;
    globjects::ref_ptr<globjects::Framebuffer>      m_fbo;
    globjects::ref_ptr<globjects::Texture>                m_colorBuffer;
    globjects::ref_ptr<globjects::Program>                m_drawProgram;
    globjects::ref_ptr<ScreenAlignedQuad> m_clear;
    globjects::ref_ptr<ScreenAlignedQuad> m_quad;
};
