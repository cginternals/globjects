
#pragma once

#include "AbstractParticleTechnique.h"


class FragmentShaderParticles : public AbstractParticleTechnique
{
public:
    FragmentShaderParticles(
        const std::vector<glm::vec4> & postions
    ,   const std::vector<glm::vec4> & velocities
    ,   const glow::Texture & forces
    ,   const glow::Camera & camera);
    virtual ~FragmentShaderParticles();

    virtual void initialize();
    virtual void reset();

    virtual void step(float elapsed);
    virtual void draw();

    virtual void resize();

};
