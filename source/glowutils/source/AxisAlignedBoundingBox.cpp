#include <glowutils/AxisAlignedBoundingBox.h>

#include <cfloat>

using namespace glm;

namespace glowutils
{

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
: m_urb(vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX))
, m_llf(vec3(+FLT_MAX, +FLT_MAX, +FLT_MAX))
, m_radius(0)
{
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3& llf, const glm::vec3 urb)
: m_urb(vec3(
    glm::max(llf.x, urb.x),
    glm::max(llf.y, urb.y),
    glm::max(llf.z, urb.z)
))
, m_llf(vec3(
    glm::min(llf.x, urb.x),
    glm::min(llf.y, urb.y),
    glm::min(llf.z, urb.z)
))
, m_center(m_llf + (m_urb - m_llf) * .5f)
, m_radius(static_cast<float>((m_urb - m_llf).length()) * .5f)
{
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
{
}

bool AxisAlignedBoundingBox::extend(const vec3 & vertex)
{
    const vec3 llf(m_llf);
    const vec3 urb(m_urb);

    if (vertex.x < m_llf.x)
        m_llf.x = vertex.x;
    if (vertex.x > m_urb.x)
        m_urb.x = vertex.x;

    if (vertex.y < m_llf.y)
        m_llf.y = vertex.y;
    if (vertex.y > m_urb.y)
        m_urb.y = vertex.y;

    if (vertex.z < m_llf.z)
        m_llf.z = vertex.z;
    if (vertex.z > m_urb.z)
        m_urb.z = vertex.z;

    const bool extended(urb != m_urb || llf != m_llf);

    if (extended)
    {
        m_center = m_llf + (m_urb - m_llf) * .5f;
        m_radius = static_cast<float>((m_urb - m_llf).length()) * .5f;
    }

    return extended;
}

const vec3 & AxisAlignedBoundingBox::center() const
{
    return m_center;
}

float AxisAlignedBoundingBox::radius() const
{
    return m_radius;
}

const vec3 & AxisAlignedBoundingBox::llf() const
{
    return m_llf;
}

const vec3 & AxisAlignedBoundingBox::urb() const
{
    return m_urb;
}

bool AxisAlignedBoundingBox::inside(const vec3 & vertex) const
{
    return vertex.x >= m_llf.x && vertex.x <= m_urb.x
    && vertex.y >= m_llf.y && vertex.y <= m_urb.y
    && vertex.z >= m_llf.z && vertex.z <= m_urb.z;
}

bool AxisAlignedBoundingBox::outside(const vec3 & vertex) const
{
    return !inside(vertex);
}
    
} // namespace glowutils
