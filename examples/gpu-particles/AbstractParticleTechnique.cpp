
#include <glow/Texture.h>
#include <glowutils/Camera.h>

#include "AbstractParticleTechnique.h"

AbstractParticleTechnique::AbstractParticleTechnique(
    const std::vector<glm::vec4> & positions
,   const std::vector<glm::vec4> & velocities
,   const glow::Texture & forces
,   const glow::Camera & camera)
: m_positions(positions)
, m_velocities(velocities)
, m_forces(forces)
, m_camera(camera)
{
}

AbstractParticleTechnique::~AbstractParticleTechnique()
{
}
