#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils_api.h>

namespace glowutils
{

class AxisAlignedBoundingBox;

namespace navigationmath
{

//void retrieveNearAndFarPlane(
//    const glm::vec3 & camera
//,   const glm::vec3 & center
//,   const AxisAlignedBoundingBox & aabb
//,   float & zNear = 0.1f
//,   float & zFar = 1024.f);

GLOWUTILS_API glm::vec3 rayPlaneIntersection(
    bool & valid
,	const glm::vec3 & rnear	///< ray start
,	const glm::vec3 & rfar  ///< ray end - used for line direction retrieval
,	const glm::vec3 & location = glm::vec3(0.f, 0.f, 0.f) ///< location of the plane
,	const glm::vec3 & normal = glm::vec3(0.f, 1.f, 0.f)); ///< normal of the plane

//glm::vec3 rayPlaneIntersectionExt(
//    const glm::vec3 & rnear ///< ray start
//,   const glm::vec3 & rfar  ///< ray end - used for line direction retrieval
//,   const glm::vec3 & location = glm::vec3(0.f, 0.f, 0.f) ///< location of the plane
//,   const glm::vec3 & normal = glm::vec3(0.f, 1.f, 0.f)   ///< normal of the plane
//,   float maxDistance = 8.0);

/** Yields the intersection point of a ray defined by a 2d point to the
    root with an axis aligned square of side length a
*/
GLOWUTILS_API glm::vec2 raySquareIntersection(
    const glm::vec2 & point ///< ray points from point to root
,	float length = 1.f);

GLOWUTILS_API bool insideSquare(
    const glm::vec2 & point ///< ray points from point to root
,	float length = 1.f);

GLOWUTILS_API glm::vec2 rayCircleIntersection(
    bool & valid
,	const glm::vec2 & origin
,	const glm::vec2 & ray
,	float radius = 1.f);

GLOWUTILS_API float angle(
    const glm::vec3 & a
,	const glm::vec3 & b);

GLOWUTILS_API bool boundaryVisible(
    const glm::mat4 & modelViewProjection
,   const AxisAlignedBoundingBox & aabb);

GLOWUTILS_API bool pointVisible(
    const glm::mat4 & modelViewProjection
,	const glm::vec3 & point);

/** Calculates a camera eye position that is either equal or further away
    from the given camera center, ensured to have the given point in view.
*/
GLOWUTILS_API glm::vec3 cameraWithPointInView(
    const glm::vec3 & eye
,	const glm::vec3 & center
,	const glm::vec3 & up
,	float verticalFieldOfView
,	float aspect
,	const glm::vec3 & point);

GLOWUTILS_API float distanceToClosestPoint(
    const glm::vec3 & camera
,	const glm::vec3 & center
,	const glm::vec3 & point);

} // namespace navigationmath

} // namespace glowutils
