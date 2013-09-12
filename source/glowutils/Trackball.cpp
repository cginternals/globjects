
#include <cmath>

#include <glm/glm.hpp>

#include <glowutils/Trackball.h>


namespace glow 
{

Trackball::Trackball()
: m_radius(1)
{
}

void Trackball::reset()
{
	m_orientation = glm::quat();
}

void Trackball::setRadius(float radius)
{
	m_radius = radius;
}

glm::quat Trackball::orientation() const
{
	return m_orientation;
}

glm::mat4 Trackball::orientationMatrix() const
{
	return glm::mat4_cast(m_orientation);
}

void Trackball::rotate(const glm::vec2& from, const glm::vec2& to, const glm::vec2& screenSize)
{
	rotate(
		glm::vec2(from.x/screenSize.x*2-1, (screenSize.y-from.y)/screenSize.y*2-1),
		glm::vec2(to.x/screenSize.x*2-1, (screenSize.y-to.y)/screenSize.y*2-1)
	);
}

void Trackball::rotate(const glm::vec2& from, const glm::vec2& to)
{
	m_orientation = m_orientation * makeRotation(mapToSphere(from), mapToSphere(to));
}

glm::vec3 Trackball::mapToSphere(const glm::vec2& pos) const
{
	return glm::normalize(glm::vec3(pos, depth(pos)));
}

float Trackball::depth(const glm::vec2& pos) const
{
	// http://www.opengl.org/wiki/Trackball
	float d = pos.x*pos.x + pos.y*pos.y;
	float r2 = m_radius*m_radius;

	return d <= r2 * .5f ?  sqrt(r2 - d) : r2 * .5f / sqrt(d);
}

glm::quat Trackball::makeRotation(const glm::vec3& from, const glm::vec3& to)
{
	return glm::quat(glm::dot(from, to), glm::cross(from, to)*m_orientation);
}

} // namespace glow
