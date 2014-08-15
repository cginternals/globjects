#pragma once

#include <glm/glm.hpp>

#include <globjects-utils/globjects-utils_api.h>

namespace gloutils 
{

float pointToPlaneDistance(
    const glm::vec3 & location
,   const glm::vec3 & normal
,   const glm::vec3 & point);

const glm::mat4 GLOBJECTS_UTILS_API rotate(
    const glm::vec3 & a
,   const glm::vec3 & b);

const glm::mat4 GLOBJECTS_UTILS_API transform(
    const glm::vec3 & location
,   const glm::vec3 & normal);

const glm::vec3 GLOBJECTS_UTILS_API intersection(
	bool & intersects
,   const glm::vec3 & location
,   const glm::vec3 & normal
,	const glm::vec3 & rnear
,	const glm::vec3 & rfar);

} // namespace gloutils
