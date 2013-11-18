#pragma once


#include "AbstractParticleTechnique.h"


namespace glow
{
    class Program;
    class Buffer;
    class TransformFeedback;
    class Texture;
    class FrameBufferObject;
    class ScreenAlignedQuad;
    class VertexArrayObject;
}


class FragmentShaderParticles : public AbstractParticleTechnique
{
public:
    FragmentShaderParticles(
        const glow::Array<glm::vec4> & positions
    ,   const glow::Array<glm::vec4> & velocities
    ,   const glow::Texture & forces
    ,   const glow::Camera & camera);
    virtual ~FragmentShaderParticles();

    virtual void initialize();
    virtual void reset();

    virtual void step(float elapsed);
    virtual void draw(float elapsed);

    virtual void resize();

protected:
    // Particle data
    glow::ref_ptr<glow::Buffer> m_sourcePositions;
    glow::ref_ptr<glow::Buffer> m_sourceVelocities;
    glow::ref_ptr<glow::VertexArrayObject> m_vao;

    // Access to vertex buffers via texture objects
    glow::ref_ptr<glow::Texture> m_sourcePositionsTexture;
    glow::ref_ptr<glow::Texture> m_sourceVelocitiesTexture;
    glow::ref_ptr<glow::Texture> m_destPositionsTexture;
    glow::ref_ptr<glow::FrameBufferObject> m_fboParticles;
    glow::ref_ptr<glow::ScreenAlignedQuad> m_update;

    // Output FBO
    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
    glow::ref_ptr<glow::Texture> m_colorBuffer;

    // Rendering
    glow::ref_ptr<glow::Program> m_drawProgram;
    glow::ref_ptr<glow::ScreenAlignedQuad> m_quad;
    glow::ref_ptr<glow::ScreenAlignedQuad> m_clear;
};
