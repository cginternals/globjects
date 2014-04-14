#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils_api.h>

namespace glowutils
{

/** \brief Axis aligned rectangular bounding volume spanned by lower left front and upper right back points.

    The AxisAlignedBoundingBox is a simple util providing additional functionality 
    to this common bounding volume. The specification of the box is intended to be 
    indirect, by extending the box vertex by vertex. The box itself keeps track of 
    its extents and provides a center of gravity, a radius, as well as inside/outside
    testes for vertices.

    Typeical usage of the AxisAlignedBoundingBox:
    \code{.cpp}

        for (vec3 vertex, vertices)
            aabb->extend(vertex);

        if(aabb->inside(otherVertex))
            ...

            \endcode
*/
class GLOWUTILS_API AxisAlignedBoundingBox
{
public:
    AxisAlignedBoundingBox();
    AxisAlignedBoundingBox(const glm::vec3& llf, const glm::vec3 urb);
    virtual ~AxisAlignedBoundingBox();

    bool extend(const glm::vec3 & vertex);

    const glm::vec3 & center() const;
    float radius() const;

    const glm::vec3 & llf() const;
    const glm::vec3 & urb() const;

    bool inside(const glm::vec3 & vertex) const;
    bool outside(const glm::vec3 & vertex) const;

protected:
    glm::vec3 m_urb;
    glm::vec3 m_llf;

    glm::vec3 m_center;
    float m_radius;
};

} // namespace glowutils
