#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils.h>

namespace glowutils 
{

float pointToPlaneDistance(
    const glm::vec3 & location
,   const glm::vec3 & normal
,   const glm::vec3 & point);

const glm::mat4 GLOWUTILS_API rotate(
    const glm::vec3 & a
,   const glm::vec3 & b);

const glm::mat4 GLOWUTILS_API transform(
    const glm::vec3 & location
,   const glm::vec3 & normal);

const glm::vec3 GLOWUTILS_API intersection(
	bool & intersects
,   const glm::vec3 & location
,   const glm::vec3 & normal
,	const glm::vec3 & rnear
,	const glm::vec3 & rfar);

} // namespace glowutils
