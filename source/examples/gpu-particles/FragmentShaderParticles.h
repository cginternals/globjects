#pragma once

#include <globjects/base/ref_ptr.h>

#include <globjects/VertexArray.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <common/ScreenAlignedQuad.h>

#include "AbstractParticleTechnique.h"


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

protected:
    virtual void draw_impl() override;

protected:
    std::vector<glm::vec4> m_positionsFilled;
    std::vector<glm::vec4> m_velocitiesFilled;

    globjects::ref_ptr<globjects::Texture> m_positionsTex;
    globjects::ref_ptr<globjects::Texture> m_velocitiesTex;

    globjects::ref_ptr<globjects::Framebuffer> m_updateFbo;
    globjects::ref_ptr<ScreenAlignedQuad> m_updateQuad;

    globjects::ref_ptr<globjects::VertexArray> m_vao;

    glm::ivec2 m_workGroupSize;
};
