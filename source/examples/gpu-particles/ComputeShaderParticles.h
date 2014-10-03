#pragma once

#include <globjects/base/ref_ptr.h>

#include <globjects/VertexArray.h>
#include <globjects/Program.h>
#include <globjects/Buffer.h>

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
    
protected:
    virtual void draw_impl() override;

protected:
    globjects::ref_ptr<globjects::Buffer> m_positionsSSBO;
    globjects::ref_ptr<globjects::Buffer> m_velocitiesSSBO;

    globjects::ref_ptr<globjects::Program> m_computeProgram;

    globjects::ref_ptr<globjects::VertexArray> m_vao;

    glm::uvec3 m_workGroupSize;
};
