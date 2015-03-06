#pragma once

#include "AbstractParticleTechnique.h"


namespace globjects
{
class Texture;
class Framebuffer;
class VertexArray;
}

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

    std::unique_ptr<globjects::Texture> m_positionsTex;
    std::unique_ptr<globjects::Texture> m_velocitiesTex;

    std::unique_ptr<globjects::Framebuffer> m_updateFbo;
    std::unique_ptr<ScreenAlignedQuad> m_updateQuad;
    std::unique_ptr<globjects::AbstractStringSource> m_updateSource;
    std::unique_ptr<globjects::Shader> m_updateShader;

    std::unique_ptr<globjects::VertexArray> m_vao;

    std::unique_ptr<globjects::Uniform<int>> m_verticesUniform;
    std::unique_ptr<globjects::Uniform<int>> m_velocitiesUniform;
    std::unique_ptr<globjects::Uniform<int>> m_forcesUniform;
    std::unique_ptr<globjects::Uniform<float>> m_elapsedUniform;
    std::unique_ptr<globjects::Uniform<int>> m_textureWidthUniform;

    glm::ivec2 m_workGroupSize;
};
