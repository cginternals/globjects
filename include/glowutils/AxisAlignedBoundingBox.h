#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils.h>

namespace glow
{

class GLOWUTILS_API AxisAlignedBoundingBox
{
public:
    AxisAlignedBoundingBox();
    virtual ~AxisAlignedBoundingBox();

    bool extend(const glm::vec3 & vertex);

    const glm::vec3 & center() const;
    float radius() const;

    const glm::vec3 & llf() const;
    const glm::vec3 & urb() const;

    bool inside(const glm::vec3 & vertex) const;
    bool outside(const glm::vec3 & vertex) const;

protected:
    glm::vec3 m_llf;
    glm::vec3 m_urb;

    glm::vec3 m_center;
    float m_radius;
};

} // namespace glow
