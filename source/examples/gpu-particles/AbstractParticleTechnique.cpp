
#include <globjects/Texture.h>
#include <common/Camera.h>

#include "AbstractParticleTechnique.h"

#include <cassert>

AbstractParticleTechnique::AbstractParticleTechnique(
    const std::vector<glm::vec4> & positions
,   const std::vector<glm::vec4> & velocities
,   const glo::Texture & forces
,   const Camera & camera)
: m_positions(positions)
, m_velocities(velocities)
, m_forces(forces)
, m_camera(camera)
, m_numParticles(static_cast<unsigned>(positions.size()))
{
    assert(positions.size() == velocities.size());
}

AbstractParticleTechnique::~AbstractParticleTechnique()
{
}
