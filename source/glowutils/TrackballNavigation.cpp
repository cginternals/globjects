#include <glowutils/TrackballNavigation.h>

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

using namespace glm;

namespace glowutils 
{

TrackballNavigation::TrackballNavigation()
: m_radius(1)
{
}

void TrackballNavigation::reset()
{
	m_orientation = quat();
}

void TrackballNavigation::setRadius(float radius)
{
	m_radius = radius;
}

quat TrackballNavigation::orientation() const
{
	return m_orientation;
}

mat4 TrackballNavigation::orientationMatrix() const
{
	return mat4_cast(m_orientation);
}

void TrackballNavigation::rotate(
    const vec2 & from
,   const vec2 & to
,   const vec2 & screenSize)
{
    rotate(
        vec2(from.x, screenSize.y - from.y) / screenSize * 2.f - 1.f
    ,   vec2(to.x, screenSize.y - to.y) / screenSize * 2.f - 1.f);
}

void TrackballNavigation::rotate(
    const vec2 & from
,   const vec2 & to)
{
	m_orientation = m_orientation * makeRotation(mapToSphere(from), mapToSphere(to));
}

vec3 TrackballNavigation::mapToSphere(const vec2 & pos) const
{
	return normalize(vec3(pos, depth(pos)));
}

float TrackballNavigation::depth(const vec2 & pos) const
{
	// http://www.opengl.org/wiki/TrackballNavigation
	float d = length2(pos);
	float r2 = m_radius * m_radius;

	return d <= r2 * .5f ? sqrt(r2 - d) : r2 * .5f / sqrt(d);
}

quat TrackballNavigation::makeRotation(
    const vec3 & from
,   const vec3& to)
{
	return quat(dot(from, to), cross(from, to) * m_orientation);
}

} // namespace glowutils
