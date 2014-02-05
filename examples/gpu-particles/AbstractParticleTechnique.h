#pragma once

#include <vector>

#include <GL/glew.h>

#include <glow/ref_ptr.h>

#include <glm/glm.hpp>


namespace glow
{
    class Texture;
}

namespace glowutils
{
    class Camera;
}

class AbstractParticleTechnique
{
public:
    AbstractParticleTechnique(
        const std::vector<glm::vec4> & postions
    ,   const std::vector<glm::vec4> & velocities
    ,   const glow::Texture & forces
    ,   const glowutils::Camera & camera);

    virtual ~AbstractParticleTechnique();

    virtual void initialize() = 0;
    virtual void reset() = 0;

    virtual void step(float elapsed) = 0;
    virtual void draw(float elapsed) = 0;

    virtual void resize() = 0; // use m_camera viewport etc.

protected:
    const std::vector<glm::vec4> & m_positions;
    const std::vector<glm::vec4> & m_velocities;

    const glow::Texture & m_forces;
    const glowutils::Camera & m_camera;

    const unsigned int m_numParticles;
};
