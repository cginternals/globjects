
#include <cassert>
#include <cfloat>

#include <glm/gtx/norm.hpp>

#include <glowutils/MathMacros.h>
#include <glowutils/AxisAlignedBoundingBox.h>

#include <glowutils/NavigationMath.h>

using namespace glm;

namespace glow
{

bool NavigationMath::validDepth(const float depth)
{
    return depth < (1.f - std::numeric_limits<float>::epsilon());
}

//void NavigationMath::retrieveNearAndFarPlane(
//    const vec3 & camera
//,   const vec3 & center
//,   const Boundary & boundary
//,   float & zNear
//,   float & zFar)
//{
//    assert(zNear > 0.);
//    assert(zFar > zNear);
//}

const vec3 NavigationMath::rayPlaneIntersection(
	bool & valid
,	const vec3 & rnear
,	const vec3 & rfar
,	const vec3 & location
,	const vec3 & normal)
{
	const vec3 r0 = rnear; // root of the ray
	const vec3 &r = rfar - rnear; 

	// intersect with plane in point normal form
	const float lDotN = dot(r, normal);

	valid = r != vec3();

	if(!valid)
		return vec3();

	float t = dot(location - r0, normal) / lDotN;
	return t * r + r0; // retrieve point via the ray
}

//const vec3 NavigationMath::rayPlaneIntersectionExt(
//	const vec3 & rnear
//,	const vec3 & rfar
//,	const vec3 & location
//,	const vec3 & normal
//,   const float maxDistance)
//{
//	bool valid;
//
//	vec3 i = rayPlaneIntersection(valid, rnear, rfar, location, normal);
//
//    if (valid && i.length() < maxDistance)
//		return i;
//
//	const vec2 r0 = xz(rnear); // root of the ray, projected onto xz-plane
//	const vec2 &r = xz(rfar - rnear);
//
//    i = -rayCircleIntersection(valid, r0, r, maxDistance);
//	return vec3(i.x(), 0.0, i.y);
//}

const vec2 NavigationMath::raySquareIntersection(
	const vec2 & point
,	const float length)
{
	const float ax = abs(point.x);
    const float ay = abs(point.y);

	if(ax >= ay) // intersection is with left or right border
        return vec2(sign(point.x), point.y / ax) * length;
	else // intersection is with bottom or top border
		return vec2(point.x / ay, sign(point.y)) * length;
}

bool NavigationMath::insideSquare(
	const vec2 & point
,	const float length)
{
	return abs(point.x) <= length && abs(point.y) <= length;
}

const vec2 NavigationMath::rayCircleIntersection(
	bool & valid
,	const vec2 & origin
,	const vec2 & ray
,	const float radius)
{
	if(ray == vec2())
	{
		valid = false;
		return vec2();
	}

	const float a = length2(ray);
	const float b = 2.f * dot(ray, origin);
    const float c = length2(origin) - static_cast<float>(radius * radius);

	const float delta = b * b - 4.f * a * c;

	if (delta < 0.f) 
	{
		valid = false;
		return vec2();
	}

	const float s = sqrt(delta);

	// the two intersections
	const float t0 = (-b + s) / (2.f * a);
	const float t1 = (-b - s) / (2.f * a);

	// nearest one
	const float t = min(t0, t1);

	valid = true;
	return t * ray + origin;
}

const vec2 NavigationMath::xz(const vec3 & xyz) 
{
	return vec2(xyz.x, xyz.z);
}

float NavigationMath::angle(
	const vec3 & a
,	const vec3 & b) 
{
	return acos(dot(normalize(a), normalize(b)));
}

bool NavigationMath::boundaryVisible(
    const mat4 & mvp
,   const AxisAlignedBoundingBox & b)
{
	const vec3 box[8] = 
	{
	// front
		vec3(b.llf().x,  b.llf().y, b.urb().z)
	,	vec3(b.urb().x,  b.llf().y, b.urb().z)
	,	vec3(b.urb().x,  b.urb().y, b.urb().z)
	,	vec3(b.llf().x,  b.urb().y, b.urb().z)
	// back
	,	vec3(b.llf().x,  b.llf().y, b.llf().z)
	,	vec3(b.urb().x,  b.llf().y, b.llf().z)
	,	vec3(b.urb().x,  b.urb().y, b.llf().z)
	,	vec3(b.llf().x,  b.urb().y, b.llf().z)
	};

	// transform bounds and check if any point is outside NDC (Normalized Device 
    // Coordinates) space 

	for(int i = 0; i < 8; ++i)
	{
        const vec3 t = vec3(mvp * vec4(box[i], 0.f));

		if(abs(t.x) > 1.f || abs(t.y) > 1.f)
			return false;
	}
	return true;
}

bool NavigationMath::pointVisible(
	const mat4 & mvp
,	const vec3 & p)
{
	const vec3 t = vec3(mvp * vec4(p, 0.f));

    return abs(t.x) <= 1.f && abs(t.y) <= 1.f;
}

const vec3 NavigationMath::cameraWithPointInView(
	const vec3 & eye
,	const vec3 & center
,	const vec3 & up
,	const float fovy
,	const float aspect
,	const vec3 & point)
{
	const vec3 ray(center - eye);
	const vec3 n(normalize(ray));

	// retrieve u and v for orthonormal basis
	const vec3 v(normalize(cross(n, up)));
    const vec3 u(normalize(cross(v, n)));

	// get both field of view vectors
	const float vFov(rad(fovy));
	const float hFov(2.f * atan(tan(vFov * .5f) * aspect));

	// closest point c
    const vec3 c = eye + ray * distanceToClosestPoint(eye, center, point);

	// project bc = t to u and v using scalar projection
	const vec3 t = point - c;

	const float uPart = abs(dot(t, u));
	const float vPart = abs(dot(t, v)) / aspect;

	// retrieve max distance to camera with required fov for the bigger part
	const float p = max(uPart, vPart);

	// required distance from c to new camera position
	const float a = p / tan(vFov * .5f);

    return c - a * n;
}

float NavigationMath::distanceToClosestPoint(
	const vec3 & eye
,	const vec3 & center
,	const vec3 & point)
{
	const vec3 ray(center - eye);
	const vec3 b(point - eye);

	const float m = length2(ray); // magnitude of ray
	const float theta = dot(b, ray);

	if(m == 0.f)
		return 0.f;

    return theta / m; // distance from camera to closest point c;
}

} // namespace glow
