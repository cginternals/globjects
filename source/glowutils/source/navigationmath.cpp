#include <glowutils/navigationmath.h>

//#include <cassert>

#include <glm/gtx/norm.hpp>

#include <glowutils/AxisAlignedBoundingBox.h>

using namespace glm;

namespace glowutils
{

namespace navigationmath
{

//void retrieveNearAndFarPlane(
//    const vec3 & camera
//,   const vec3 & center
//,   const Boundary & boundary
//,   float & zNear
//,   float & zFar)
//{
//    assert(zNear > 0.);
//    assert(zFar > zNear);
//}

vec3 rayPlaneIntersection(
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

//vec3 rayPlaneIntersectionExt(
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

vec2 raySquareIntersection(
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

bool insideSquare(
	const vec2 & point
,	const float length)
{
	return abs(point.x) <= length && abs(point.y) <= length;
}

vec2 rayCircleIntersection(
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
    const float c = length2(origin) - radius * radius;

	const float delta = b * b - 4.f * a * c;

	if (delta < 0.f) 
	{
		valid = false;
		return vec2();
	}

	const float s = std::sqrt(delta);

	// the two intersections
	const float t0 = (-b + s) / (2.f * a);
	const float t1 = (-b - s) / (2.f * a);

	// nearest one
	const float t = min(t0, t1);

	valid = true;
	return t * ray + origin;
}

float angle(
	const vec3 & a
,	const vec3 & b) 
{
	return acos(dot(normalize(a), normalize(b)));
}

bool boundaryVisible(
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

bool pointVisible(
	const mat4 & mvp
,	const vec3 & p)
{
	const vec3 t = vec3(mvp * vec4(p, 0.f));

    return glm::abs(t.x) <= 1.f && glm::abs(t.y) <= 1.f;
}

vec3 cameraWithPointInView(
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
    const float vFov(glm::radians(fovy));
    //const float hFov(2.f * atan(tan(vFov * .5f) * aspect));

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

float distanceToClosestPoint(
	const vec3 & eye
,	const vec3 & center
,	const vec3 & point)
{
	const vec3 ray(center - eye);
	const vec3 b(point - eye);

	const float m = length2(ray); // magnitude of ray
	const float theta = dot(b, ray);

    if(std::abs(m) < std::numeric_limits<float>::epsilon())
		return 0.f;

    return theta / m; // distance from camera to closest point c;
}

} // namespace navigationamth

} // namespace glowutils
