#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils.h>


namespace glowutils
{
class AxisAlignedBoundingBox;

class GLOWUTILS_API NavigationMath
{
public:
    //static void retrieveNearAndFarPlane(
    //    const glm::vec3 & camera
    //,   const glm::vec3 & center
    //,   const AxisAlignedBoundingBox & aabb
    //,   float & zNear = 0.1f
    //,   float & zFar = 1024.f);

	static const glm::vec3 rayPlaneIntersection(
		bool & valid
	,	const glm::vec3 & rnear	///< ray start
	,	const glm::vec3 & rfar  ///< ray end - used for line direction retrieval
	,	const glm::vec3 & location = glm::vec3(0.f, 0.f, 0.f) ///< location of the plane
	,	const glm::vec3 & normal = glm::vec3(0.f, 1.f, 0.f)); ///< normal of the plane

    //static const glm::vec3 rayPlaneIntersectionExt(
    //    const glm::vec3 & rnear ///< ray start
    //,   const glm::vec3 & rfar  ///< ray end - used for line direction retrieval
    //,   const glm::vec3 & location = glm::vec3(0.f, 0.f, 0.f) ///< location of the plane
    //,   const glm::vec3 & normal = glm::vec3(0.f, 1.f, 0.f)   ///< normal of the plane
    //,   float maxDistance = 8.0);

	/** Yields the intersection point of a ray defined by a 2d point to the 
        root with an axis aligned square of side length a
    */
    static const glm::vec2 raySquareIntersection(
		const glm::vec2 & point ///< ray points from point to root
	,	float length = 1.f);

	static bool insideSquare(
		const glm::vec2 & point ///< ray points from point to root
	,	float length = 1.f);

	static const glm::vec2 rayCircleIntersection(
		bool & valid
	,	const glm::vec2 & origin
	,	const glm::vec2 & ray
	,	float radius = 1.f);

	static float angle(
		const glm::vec3 & a
	,	const glm::vec3 & b);

	static bool boundaryVisible(
		const glm::mat4 & modelViewProjection
    ,   const AxisAlignedBoundingBox & aabb);

	static bool pointVisible(
		const glm::mat4 & modelViewProjection
	,	const glm::vec3 & point);

    /** Calculates a camera eye position that is either equal or further away
        from the given camera center, ensured to have the given point in view.
    */
	static const glm::vec3 cameraWithPointInView(
		const glm::vec3 & eye
	,	const glm::vec3 & center
	,	const glm::vec3 & up
	,	float verticalFieldOfView
	,	float aspect
	,	const glm::vec3 & point);

	static float distanceToClosestPoint(
		const glm::vec3 & camera
	,	const glm::vec3 & center
	,	const glm::vec3 & point);
};

} // namespace glowutils
