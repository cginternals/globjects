
#pragma once

#include <GL/glew.h>

#include <glow/ref_ptr.h>
#include <glow/Array.h>

#include <glm/glm.hpp>


namespace glow
{
    class Camera;
    class Texture;
}

class AbstractParticleTechnique
{
public:
    AbstractParticleTechnique(
        const glow::Array<glm::vec4> & postions
    ,   const glow::Array<glm::vec4> & velocities
    ,   const glow::Texture & forces
    ,   const glow::Camera & camera);

    virtual ~AbstractParticleTechnique();

    virtual void initialize() = 0;
    virtual void reset() = 0;

    virtual void step(float elapsed) = 0;
    virtual void draw() = 0;

    virtual void resize() = 0; // use m_camera viewport etc.

protected:
    const glow::Array<glm::vec4> & m_positions;
    const glow::Array<glm::vec4> & m_velocities;

    const glow::Texture & m_forces;
    const glow::Camera & m_camera;

    const unsigned int m_numParticles;
};
