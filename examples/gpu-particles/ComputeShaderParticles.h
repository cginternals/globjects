
#pragma once

#include <glow/ref_ptr.h>

#include "AbstractParticleTechnique.h"

namespace glow
{  
    class Program;
    class Buffer;
    class FrameBufferObject;
    class Texture;
    class RenderBufferObject;
    class ScreenAlignedQuad;
}


class ComputeShaderParticles : public AbstractParticleTechnique
{
public:
    ComputeShaderParticles(
        const glow::Array<glm::vec4> & positions
    ,   const glow::Array<glm::vec4> & velocities
    ,   const glow::Texture & forces
    ,   const glow::Camera & camera);
    virtual ~ComputeShaderParticles();

    virtual void initialize();
    virtual void reset();

    virtual void step(float elapsed);
    virtual void draw();

    virtual void resize();

protected:
    glow::ref_ptr<glow::Buffer> m_positionsSSBO;
    glow::ref_ptr<glow::Buffer> m_velocitiesSSBO;

    glow::ref_ptr<glow::Program> m_computeProgram;
    glow::ref_ptr<glow::Program> m_drawProgram;

    glow::ref_ptr<glow::VertexArrayObject> m_vao;

    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
    glow::ref_ptr<glow::Texture> m_color;

    glow::ScreenAlignedQuad * m_quad;
    glow::ScreenAlignedQuad * m_clear;
};
