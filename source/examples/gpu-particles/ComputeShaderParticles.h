#pragma once

#include <glowbase/ref_ptr.h>

#include "AbstractParticleTechnique.h"

namespace glow
{  
    class Program;
    class Buffer;
    class FrameBufferObject;
    class Texture;
    class RenderBufferObject;
    class VertexArrayObject;
}

namespace glowutils
{
    class ScreenAlignedQuad;
}

class ComputeShaderParticles : public AbstractParticleTechnique
{
public:
    ComputeShaderParticles(
        const std::vector<glm::vec4> & positions
    ,   const std::vector<glm::vec4> & velocities
    ,   const glow::Texture & forces
    ,   const glowutils::Camera & camera);
    virtual ~ComputeShaderParticles();

    virtual void initialize() override;
    virtual void reset() override;

    virtual void step(float elapsed) override;
    virtual void draw(float elapsed) override;

    virtual void resize() override;

protected:
    glow::ref_ptr<glow::Buffer> m_positionsSSBO;
    glow::ref_ptr<glow::Buffer> m_velocitiesSSBO;

    glow::ref_ptr<glow::Program> m_computeProgram;
    glow::ref_ptr<glow::Program> m_drawProgram;

    glow::ref_ptr<glow::VertexArrayObject> m_vao;

    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
    glow::ref_ptr<glow::Texture> m_color;

    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_clear;

    glm::uvec3 m_workGroupSize;
};
