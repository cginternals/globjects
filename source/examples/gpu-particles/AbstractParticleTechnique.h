#pragma once

#include <vector>



#include <globjects-base/ref_ptr.h>

#include <glm/glm.hpp>


namespace glo
{
    class Texture;
}

namespace gloutils
{
    class Camera;
}

class AbstractParticleTechnique
{
public:
    AbstractParticleTechnique(
        const std::vector<glm::vec4> & postions
    ,   const std::vector<glm::vec4> & velocities
    ,   const glo::Texture & forces
    ,   const gloutils::Camera & camera);

    virtual ~AbstractParticleTechnique();

    virtual void initialize() = 0;
    virtual void reset() = 0;

    virtual void step(float elapsed) = 0;
    virtual void draw(float elapsed) = 0;

    virtual void resize() = 0; // use m_camera viewport etc.

protected:
    const std::vector<glm::vec4> & m_positions;
    const std::vector<glm::vec4> & m_velocities;

    const glo::Texture & m_forces;
    const gloutils::Camera & m_camera;

    const unsigned int m_numParticles;
};
