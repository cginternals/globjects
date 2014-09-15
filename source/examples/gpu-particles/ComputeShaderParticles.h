#pragma once

#include <globjects/base/ref_ptr.h>

#include <globjects/VertexArray.h>
#include <globjects/Program.h>
#include <globjects/Buffer.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <common/ScreenAlignedQuad.h>

#include "AbstractParticleTechnique.h"

class ComputeShaderParticles : public AbstractParticleTechnique
{
public:
    ComputeShaderParticles(
        const std::vector<glm::vec4> & positions
    ,   const std::vector<glm::vec4> & velocities
    ,   const globjects::Texture & forces
    ,   const Camera & camera);
    virtual ~ComputeShaderParticles();

    virtual void initialize() override;
    virtual void reset() override;

    virtual void step(float elapsed) override;
    virtual void draw(float elapsed) override;

    virtual void resize() override;

protected:
    globjects::ref_ptr<globjects::Buffer> m_positionsSSBO;
    globjects::ref_ptr<globjects::Buffer> m_velocitiesSSBO;

    globjects::ref_ptr<globjects::Program> m_computeProgram;
    globjects::ref_ptr<globjects::Program> m_drawProgram;

    globjects::ref_ptr<globjects::VertexArray> m_vao;

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<globjects::Texture> m_color;

    globjects::ref_ptr<ScreenAlignedQuad> m_quad;
    globjects::ref_ptr<ScreenAlignedQuad> m_clear;

    glm::uvec3 m_workGroupSize;
};
