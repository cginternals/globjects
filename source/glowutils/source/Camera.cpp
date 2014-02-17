#include <glowutils/Camera.h>

#include <cassert>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace glm;

namespace glowutils
{

Camera::Camera(
    const vec3 & eye
,   const vec3 & center
,   const vec3 & up)
: m_dirty(false)

, m_eye(eye)
, m_center(center)
, m_up(up)

, m_fovy(40.f)
, m_aspect(1.f)
, m_zNear(0.1f)
, m_zFar(64.0f)
{
}

Camera::~Camera()
{
}

void Camera::invalidateMatrices() const
{
    m_view.invalidate();
    m_viewInverted.invalidate();
    m_projection.invalidate();
    m_projectionInverted.invalidate();
    m_viewProjection.invalidate();
    m_viewProjectionInverted.invalidate();
    m_normal.invalidate();
}

void Camera::dirty(bool update)
{
    m_dirty = true;

    if (update)
        this->update();
}

const vec3 & Camera::eye() const
{
    return m_eye;
}

void Camera::setEye(const vec3 & eye)
{
    if (eye == m_eye)
        return;

    m_eye = eye;
    dirty();
}

const vec3 & Camera::center() const
{
    return m_center;
}

void Camera::setCenter(const vec3 & center)
{
    if (center == m_center)
        return;

    m_center = center;
    dirty();
}

const vec3 & Camera::up() const
{
    return m_up;
}

void Camera::setUp(const vec3 & up)
{
    if (up == m_up)
        return;

    m_up = up;
    dirty();
}

float Camera::zNear() const
{
    return m_zNear;
}

void Camera::setZNear(const float zNear)
{
    if (std::abs(zNear - m_zNear) < std::numeric_limits<float>::epsilon())
        return;

    m_zNear = zNear;
    assert(m_zNear > 0.0);

    dirty();
}

float Camera::zFar() const
{
    return m_zFar;
}

void Camera::setZFar(const float zFar)
{
    if (std::abs(zFar - m_zFar) < std::numeric_limits<float>::epsilon())
        return;

    m_zFar = zFar;
    assert(m_zFar > m_zNear);

    dirty();
}

float Camera::fovy() const
{
    return m_fovy;
}

void Camera::setFovy(const float fovy)
{
    if (std::abs(fovy - m_fovy) < std::numeric_limits<float>::epsilon())
        return;

    m_fovy = fovy;
    assert(m_fovy > 0.0);

    dirty();
}

const ivec2 & Camera::viewport() const
{
    return m_viewport;
}

void Camera::setViewport(int width, int height)
{
    return setViewport(ivec2(width, height));
}

void Camera::setViewport(const ivec2 & viewport)
{
    if (viewport == m_viewport)
        return;

    m_aspect = static_cast<float>(viewport.x) / max(static_cast<float>(viewport.y), 1.f);
    m_viewport = viewport;

    dirty();
}

float Camera::aspectRatio() const
{
    return m_aspect;
}

void Camera::update() const
{
    if (!m_dirty)
        return;

    invalidateMatrices();

    m_dirty = false;

    changed();
}

const mat4 & Camera::view() const
{
    if (m_dirty)
        update();
    
    if (!m_view.isValid())
        m_view.setValue(lookAt(m_eye, m_center, m_up));

    return m_view.value();
}

const mat4 & Camera::projection() const
{
    if (m_dirty)
        update();

    if (!m_projection.isValid())
	    m_projection.setValue(perspective(m_fovy, m_aspect, m_zNear, m_zFar));

    return m_projection.value();
}

const mat4 & Camera::viewProjection() const
{
    if (m_dirty)
        update();

    if (!m_viewProjection.isValid())
    	m_viewProjection.setValue(projection() * view());
    
    return m_viewProjection.value();
}

const mat4 & Camera::viewInverted() const
{
    if (m_dirty)
        update();

    if (!m_viewInverted.isValid())
        m_viewInverted.setValue(inverse(view()));

    return m_viewInverted.value();
}

const mat4 & Camera::projectionInverted() const
{
    if (m_dirty)
        update();

    if (!m_projectionInverted.isValid())
        m_projectionInverted.setValue(inverse(projection()));

    return m_projectionInverted.value();
}

const mat4 & Camera::viewProjectionInverted() const
{
    if (m_dirty)
        update();

    if (!m_viewProjectionInverted.isValid())
        m_viewProjectionInverted.setValue(inverse(viewProjection()));

    return m_viewProjectionInverted.value();
}

const mat3 & Camera::normal() const
{
    if (m_dirty)
        update();

    if (!m_normal.isValid())
        m_normal.setValue(inverseTranspose(mat3(view())));

    return m_normal.value();
}

void Camera::changed() const
{
}

} // namespace glowutils
