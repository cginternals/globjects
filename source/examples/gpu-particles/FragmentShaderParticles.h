#pragma once


#include "AbstractParticleTechnique.h"


namespace glo
{
    class Program;
    class Texture;
    class VertexArrayObject;
    class FrameBufferObject;
}
namespace gloutils
{
    class ScreenAlignedQuad;
}


class FragmentShaderParticles : public AbstractParticleTechnique
{
public:
    FragmentShaderParticles(
        const std::vector<glm::vec4> & positions
    ,   const std::vector<glm::vec4> & velocities
    ,   const glo::Texture & forces
    ,   const gloutils::Camera & camera);
    virtual ~FragmentShaderParticles();

    virtual void initialize() override;
    virtual void reset() override;

    virtual void step(float elapsed) override;
    virtual void draw(float elapsed) override;

    virtual void resize() override;

protected:
    // Particle data
    glo::ref_ptr<glo::Texture>                m_texPositions;
    glo::ref_ptr<glo::Texture>                m_texVelocities;
    int                                         m_width;
    int                                         m_height;

    // Update of particles
    glo::ref_ptr<glo::FrameBufferObject>      m_fboUpdate;
    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_quadUpdate;

    // Rendering
    glo::ref_ptr<glo::VertexArrayObject>      m_vao;
    glo::ref_ptr<glo::FrameBufferObject>      m_fbo;
    glo::ref_ptr<glo::Texture>                m_colorBuffer;
    glo::ref_ptr<glo::Program>                m_drawProgram;
    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_clear;
    glo::ref_ptr<gloutils::ScreenAlignedQuad> m_quad;
};
