#include <glowutils/WorldInHandNavigation.h>

#include <cassert>
#include <algorithm>
#include <limits>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/constants.hpp>

#include <glow/logging.h>

#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/Camera.h>
#include <glowutils/navigationmath.h>

using namespace glm;

namespace
{
    static const vec3 DEFAULT_EYE    = vec3(0.f, 1.2f, 2.4f);
    static const vec3 DEFAULT_CENTER = vec3(0.f, 0.0f, 0.0f);
    static const vec3 DEFAULT_UP     = vec3(0.f, 1.0f, 0.0f);

//  static const float DEFAULT_SCALE_STEP = 0.1f;
    static const float DEFAULT_DISTANCE   = 2.0f;
    static const float DEFAULT_DIST_MIN   = 0.1f;
    static const float DEFAULT_DIST_MAX   = 4.0f;

    static const float ROTATION_HOR_DOF   = 0.8f * glm::pi<float>();
    static const float ROTATION_VER_DOF   = 0.8f * glm::pi<float>();

//  static const float ROTATION_KEY_SCALE = 1.0f;

    //static const float NAV_CONSTRAINT_PAN_CIRCLE_R = 2.83;
    static const float CONSTRAINT_ROT_MAX_V_UP = 0.02f * glm::pi<float>();
    static const float CONSTRAINT_ROT_MAX_V_LO = 0.98f * glm::pi<float>();
}

namespace glowutils
{

WorldInHandNavigation::WorldInHandNavigation()
: m_camera(nullptr)
, m_coordsProvider(nullptr)
, m_rotationHappened(false)
, m_mode(NoInteraction)
, m_homeEye(DEFAULT_EYE)
, m_homeCenter(DEFAULT_CENTER)
, m_homeUp(DEFAULT_UP)
, m_i0Valid(false)
{
}

WorldInHandNavigation::~WorldInHandNavigation()
{
}

WorldInHandNavigation::InteractionMode WorldInHandNavigation::mode() const
{
    return m_mode;
}

void WorldInHandNavigation::setBoundaryHint(const AxisAlignedBoundingBox & aabb)
{
    m_aabb = aabb;
}

void WorldInHandNavigation::setCoordinateProvider(AbstractCoordinateProvider * provider)
{
    m_coordsProvider = provider;
}

void WorldInHandNavigation::setCamera(Camera * camera)
{
    m_camera = camera;

    if (camera)
    {
        m_homeEye = camera->eye();
        m_homeCenter = camera->center();
        m_homeUp = camera->up();
    }
}

void WorldInHandNavigation::reset()
{
    if (!m_camera)
        return;

    m_camera->setEye(m_homeEye);
    m_camera->setCenter(m_homeCenter);
    m_camera->setUp(m_homeUp);

    m_mode = NoInteraction;

//    enforceWholeMapVisible();
}


const vec3 WorldInHandNavigation::mouseRayPlaneIntersection(
    bool & intersects
,   const ivec2 & mouse
,   const vec3 & p0) const
{
    if (!m_coordsProvider)
        return vec3();

    // build a ray in object space from screen space mouse position and get
    // intersection with near and far planes.

    const vec3 ln = m_coordsProvider->objAt(mouse, 0.0);
    const vec3 lf = m_coordsProvider->objAt(mouse, 1.0);

    return navigationmath::rayPlaneIntersection(intersects, ln, lf, p0);
}

const vec3 WorldInHandNavigation::mouseRayPlaneIntersection(
    bool & intersects
,   const ivec2 & mouse
,   const vec3 & p0
,   const mat4 & viewProjectionInverted) const
{
    if (!m_coordsProvider)
        return vec3();

    // build a ray in object space from screen space mouse position and get
    // intersection with near and far planes.

    const vec3 ln = m_coordsProvider->objAt(mouse, 0.0, viewProjectionInverted);
    const vec3 lf = m_coordsProvider->objAt(mouse, 1.0, viewProjectionInverted);

    return navigationmath::rayPlaneIntersection(intersects, ln, lf, p0);
}

const vec3 WorldInHandNavigation::mouseRayPlaneIntersection(
    bool & intersects
,   const ivec2 & mouse) const
{
    if (!m_coordsProvider)
        return vec3();

    const float depth = m_coordsProvider->depthAt(mouse);
    const bool valid = AbstractCoordinateProvider::validDepth(depth);

    // no scene object was picked - simulate picking on xz-plane
    if (!valid)
        return mouseRayPlaneIntersection(intersects, mouse, vec3());

    return m_coordsProvider->objAt(mouse, depth);
}

void WorldInHandNavigation::panBegin(const ivec2 & mouse)
{
    if (NoInteraction != m_mode || !m_camera || !m_coordsProvider)
        return;

    m_mode = PanInteraction;
    m_viewProjectionInverted = m_camera->viewProjectionInverted();
    
    bool intersects;
    m_i0 = mouseRayPlaneIntersection(intersects, mouse);

    if (intersects)
    {
        const float depth = m_coordsProvider->depthAt(mouse);
        m_i0Valid = AbstractCoordinateProvider::validDepth(depth);
    }
    m_i0Valid = false;

    m_eye = m_camera->eye();
    m_center = m_camera->center();
}

void WorldInHandNavigation::panEnd()
{
    if (PanInteraction != m_mode)
        return;

    m_mode = NoInteraction;
}

void WorldInHandNavigation::panProcess(const ivec2 & mouse)
{
    if (PanInteraction != m_mode || !m_coordsProvider || !m_camera)
        return;

    // The first click of the interaction yields a object space position m_i0.
    // this point is our constraint for panning, that means for every mouse 
    // position there has to be an appropriate positioning for the scene, so
    // that the point under the mouse remains m_i0.
    // With this point and the up normal we build a plane, that defines the 
    // panning space. For panning, a ray is created, pointing from the screen
    // pixel into the view frustum. This ray then is converted to object space
    // and used to intersect with the plane at p. 
    // The delta of m_i0 and p is the translation required for panning.

    // constrain mouse interaction to viewport (if disabled, could lead to mishaps)
    const ivec2 clamped(
        glm::clamp(mouse.x, 0, m_camera->viewport().x)
    ,   glm::clamp(mouse.y, 0, m_camera->viewport().y));

    bool intersects;
    m_i1 = mouseRayPlaneIntersection(intersects, clamped, m_i0, m_viewProjectionInverted);

    if (intersects)
        pan(m_i0 - m_i1);
}

void WorldInHandNavigation::pan(vec3 t)
{
    if (PanInteraction != m_mode || !m_camera)
        return;

    //enforceTranslationConstraints(t);
	if (glm::isinf(t.x) || glm::isinf(t.y) || glm::isinf(t.z)) {
		return;
	}
	
    m_camera->setEye(t + m_eye);
    m_camera->setCenter(t + m_center);

    m_camera->update();
}


void WorldInHandNavigation::rotateBegin(const ivec2 & mouse)
{
    if (NoInteraction != m_mode)
        return;

    m_mode = RotateInteraction;

    bool intersects;
    m_i0 = mouseRayPlaneIntersection(intersects, mouse);
    m_i0Valid = intersects && AbstractCoordinateProvider::validDepth(m_coordsProvider->depthAt(mouse));

    m_m0 = mouse;

    m_eye = m_camera->eye();
    m_center = m_camera->center();
}

void WorldInHandNavigation::rotateEnd()
{
    if (RotateInteraction != m_mode)
        return;

    m_mode = NoInteraction;
}

void WorldInHandNavigation::rotateProcess(const ivec2 & mouse)
{
    if (RotateInteraction != m_mode)
        return;

    const vec2 delta(m_m0 - mouse);
    // setup the degree of freedom for horizontal rotation within a single action
    const float wDeltaX = glm::degrees(delta.x / static_cast<float>(m_camera->viewport().x));
    // setup the degree of freedom for vertical rotation within a single action
    const float wDeltaY = glm::degrees(delta.y / static_cast<float>(m_camera->viewport().y));

    rotate(wDeltaX, wDeltaY);
}

void WorldInHandNavigation::rotate(
    float hAngle
,   float vAngle)
{
    static const vec3 up(0.f, 1.f, 0.f);

    m_rotationHappened = true;

    const vec3 ray(normalize(m_camera->center() - m_eye));
    const vec3 rotAxis(cross(ray, up));

    hAngle *= ROTATION_HOR_DOF;
    vAngle *= ROTATION_VER_DOF;

    enforceRotationConstraints(hAngle, vAngle);

    vec3 t = m_i0Valid ? m_i0 : m_center;

    mat4 transform = translate(mat4(), t);
    transform = glm::rotate(transform, hAngle, up);
    transform = glm::rotate(transform, vAngle, rotAxis);
    transform = translate(transform, -t);

    m_camera->setEye(vec3(transform * vec4(m_eye, 0.f)));
    m_camera->setCenter(vec3(transform * vec4(m_center, 0.f)));

    m_camera->update();
}


void WorldInHandNavigation::scaleAtMouse(
    const ivec2 & mouse
,   float scaleDelta)
{
    const vec3 ln = m_camera->eye();
    const vec3 lf = m_camera->center();

    bool intersects;

    vec3 i = mouseRayPlaneIntersection(intersects, mouse);

    if (!intersects && !AbstractCoordinateProvider::validDepth(m_coordsProvider->depthAt(mouse)))
        return;

    // scale the distance between the pointed position in the scene and the 
    // camera position - using ray constraints, the center is scaled appropriately.

    float scale = scaleDelta;

    if (scale > 0.f)
        scale = 1.f / (1.f - scale) - 1.f; // ensure that scaling consistent in both direction

    // enforceScaleConstraints(scale, i);

    const vec3 eye = ln + scale * (ln - i);
    m_camera->setEye(eye);

    // the center needs to be constrained to the ground plane, so calc the new
    // center based on the intersection with the scene and use this to obtain 
    // the new viewray-groundplane intersection as new center.
    const vec3 center = lf + scale * (lf - i);

    m_camera->setCenter(navigationmath::rayPlaneIntersection(intersects, eye, center));
    m_camera->update();
}

void WorldInHandNavigation::resetScaleAtMouse(const ivec2 & mouse)
{
    const vec3 ln = m_camera->eye();
    const vec3 lf = m_camera->center();

    // set the distance between pointed position in the scene and camera to 
    // default distance
    bool intersects;
    vec3 i = mouseRayPlaneIntersection(intersects, mouse);
    if (!intersects && !AbstractCoordinateProvider::validDepth(m_coordsProvider->depthAt(mouse)))
        return;

    float scale = (DEFAULT_DISTANCE / static_cast<float>((ln - i).length()));

    //enforceScaleConstraints(scale, i);

    m_camera->setEye(i - scale * (i - ln));
    m_camera->setCenter(i - scale * (i - lf));

    m_camera->update();
}

void WorldInHandNavigation::scaleAtCenter(float scale)
{
    const vec3 ln = m_camera->eye();
    const vec3 lf = m_camera->center();

    bool intersects;
    vec3 i = navigationmath::rayPlaneIntersection(intersects, ln, lf);
    if (!intersects)
        return;

    //enforceScaleConstraints(scale, i);

    m_camera->setEye(ln + scale * (ln - i));
    m_camera->setCenter(lf + scale * (lf - i));

    m_camera->update();
}


void WorldInHandNavigation::enforceTranslationConstraints(vec3 & p) const
{
    mat4 m;
    translate(m, p);

    const vec2 center(vec3(m * vec4(m_center, 0.f)));
    if (navigationmath::insideSquare(center))
        return;

    const vec2 i = navigationmath::raySquareIntersection(center);
    p = vec3(i.x, 0.f, i.y) - m_center;
}

void WorldInHandNavigation::enforceRotationConstraints(float & /*hAngle*/, float & vAngle) const
{
    // hAngle is not constrained, vAngle is.

    // retrieve the angle between camera-center to up and test how much closer
    // to up/down it can be rotated and clamp if required.

    static const vec3 up(0.f, 1.f, 0.f);
    const float va = glm::degrees(glm::acos(dot(normalize(m_eye - m_center), up)));

    if (vAngle <= 0.f)
        vAngle = std::max(vAngle, glm::degrees(CONSTRAINT_ROT_MAX_V_UP) - va);
    else
        vAngle = std::min(vAngle, glm::degrees(CONSTRAINT_ROT_MAX_V_LO) - va);
}
 
void WorldInHandNavigation::enforceScaleConstraints(
    float & scale
,   vec3 & i) const
{
    // first constraint: i must be within the ground quad...
    vec2 i2(i);

    if (!navigationmath::insideSquare(i2))
    {
        i2 = navigationmath::raySquareIntersection(i2);
        i = vec3(i2.x, 0.f, i2.y);
    }

    // second constraint: scale factor must be within min and max... 
    const vec3 eye = m_eye + scale * (m_eye - i);

    const float ds = length(eye - i);

    if ((scale > 0.f && ds >= DEFAULT_DIST_MAX)
    ||  (scale < 0.f && ds <= DEFAULT_DIST_MIN)
    ||  (eye.y <= m_center.y))	// last fixes abnormal scales (e.g., resulting from mouse flywheels)
        scale = 0.f;
}

//void WorldInHandNavigation::enforceWholeMapVisible(const float offset)
//{
//    const float h(_swmBounds.urb.y);
//
//    const vec3 bbox[8] =
//    {
//        // front
//        vec3(-1.f, 0.f, +1.f)
//        , vec3(+1.f, 0.f, +1.f)
//        , vec3(+1.f, h, +1.f)
//        , vec3(-1.f, h, +1.f)
//        // back
//        , vec3(-1.f, 0.f, -1.f)
//        , vec3(+1.f, 0.f, -1.f)
//        , vec3(+1.f, h, -1.f)
//        , vec3(-1.f, h, -1.f)
//    };
//
//    float nearest = FLT_MAX;
//
//    vec3 farthestCamera = _center;
//    float farthestDistanceSquared = 0.0;
//
//    // temporaries for modelview matrix update
//    mat4 modelView, projection, modelViewProjection;
//    float zNear, zFar;
//
//    // retrieve the closest point to the ray in view direction
//    for (int i = 0; i < 8; ++i)
//    {
//        const vec3 &p(bbox[i]);
//
//        // check if the point is already visible, if not adjust camera
//        updateMatrices(farthestCamera, _center, _up, h,
//            fieldOfView(), _aspect, zNear, zFar,
//            modelView, projection, modelViewProjection);
//
//        if (NavigationUtils::pointIsVisible(modelViewProjection, p))
//            continue;
//
//        // so request new camera pos
//        const vec3 newCamera = NavigationUtils::cameraWithPointInView(
//            _camera, _center, _up, fieldOfView(), _aspect, p);
//
//        const float ls = (_center - newCamera).lengthSquared();
//
//        // retrieve distance from current center
//        if (ls > farthestDistanceSquared)
//        {
//            farthestDistanceSquared = ls;
//            farthestCamera = newCamera;
//        }
//    }
//
//    // adjust distance, so that znear is always max distance to new camera
//    float d = sqrt(farthestDistanceSquared) + offset;
//
//    d = qMin<float>(d, NAV_DEFAULT_DIST_MAX);
//
//    _camera = _center + (_camera - _center).normalized() * d;
//
//    setDirty();
//}

} // namespace glowutils
