#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

#include <glowutils/glowutils.h>

namespace glowutils 
{

class GLOWUTILS_API TrackballNavigation
{
public:
	TrackballNavigation();

	void rotate(
        const glm::vec2 & from
    ,   const glm::vec2 & to
    ,   const glm::vec2 & screenSize);
	void rotate(
        const glm::vec2 & from
    ,   const glm::vec2 & to);

	glm::quat orientation() const;
	glm::mat4 orientationMatrix() const;

	void reset();

	void setRadius(float radius);

protected:
    glm::vec3 mapToSphere(const glm::vec2 & pos) const;
    float depth(const glm::vec2 & pos) const;

    glm::quat makeRotation(
        const glm::vec3 & from
    ,   const glm::vec3 & to);

protected:
	float m_radius;
	glm::quat m_orientation;
};

} // namespace glowutils
