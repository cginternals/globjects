#include <glowutils/AxonometricLookAt.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <glow/logging.h>

using namespace glm;

namespace glowutils
{

AxonometricLookAt::AxonometricLookAt(
    ProjectionType type
,   const vec3 & position
,   float zoom
,   const mat4 & rotation)
:   m_rotation(rotation)
,   m_position(position)
,   m_zoom(zoom)
,   m_type(Custom)
,   m_rebuild(true)
{
    setType(type);
}

AxonometricLookAt::AxonometricLookAt(
    float verticalAngle
,   float horizontalAngle
,   const vec3 & position
,   float zoom
,   const mat4 & rotation)
:   m_verticalAngle(verticalAngle)
,   m_horizontalAngle(horizontalAngle)
,   m_rotation(rotation)
,   m_position(position)
,   m_zoom(zoom)
,   m_type(Custom)
,   m_rebuild(true)
{
}

void AxonometricLookAt::setType(ProjectionType type)
{
    if (m_type == type)
        return;

    m_type = type;

    switch(m_type)
    {
    case Isometric30:
        m_verticalAngle   = 30.f;
        m_horizontalAngle = 35.f;
        break;

    case Isometric12:
        m_verticalAngle   = 27.f;
        m_horizontalAngle = 30.f;
        break;

    case Military:
        m_verticalAngle   = 45.f;
        m_horizontalAngle = 45.f;
        break;

    case Dimetric427:
        m_verticalAngle   = 70.f;
        m_horizontalAngle = 20.f;
        break;

    case Chinese:
        m_verticalAngle   = 75.f;
        m_horizontalAngle = 10.f;
        break;

    case Custom:
        break;
    };
    m_rebuild = true;
}

AxonometricLookAt::ProjectionType AxonometricLookAt::type() const
{
    return m_type;
}

void AxonometricLookAt::setVerticalAngle(float angle)
{
    if(std::abs(m_verticalAngle - angle) < std::numeric_limits<float>::epsilon())
        return;

    m_verticalAngle = angle;
    m_type = Custom;
    m_rebuild = true;
}

float AxonometricLookAt::verticalAngle() const
{ 
    return m_verticalAngle;
}

void AxonometricLookAt::setHorizontalAngle(float angle)
{
    if (std::abs(m_horizontalAngle - angle) < std::numeric_limits<float>::epsilon())
        return;

    m_horizontalAngle = angle;
    m_type = Custom;
    m_rebuild = true;
}


float AxonometricLookAt::horizontalAngle() const
{ 
    return m_horizontalAngle;
}

void AxonometricLookAt::setPosition(const vec3 & position)
{
    if (m_position == position)
        return;

    m_position = position;
    m_rebuild = true;
}

const vec3 & AxonometricLookAt::position() const
{
    return m_position;
}

void AxonometricLookAt::setZoom(float zoom)
{
    if (zoom <= 0.f)
    {   
        glow::warning() << "Axonometric Look At zoom was set to 0.f (" << zoom << ")";
        zoom = 0.f;
    }

    if (std::abs(m_zoom - zoom) < std::numeric_limits<float>::epsilon())
        return;

    m_zoom = zoom;
    m_rebuild = true;
}

float AxonometricLookAt::zoom() const
{
    return m_zoom;
}

void AxonometricLookAt::setRotation(const mat4 & rotation)
{
    if (m_rotation == rotation)
        return;

    m_rotation = rotation;
    m_rebuild = true;
}

void AxonometricLookAt::rebuild() const
{
    if(!m_rebuild)
        return;

    const mat4 vrot = mat4_cast(angleAxis(-m_verticalAngle, vec3( 0.f, 1.f, 0.f)));
    const mat4 hrot = mat4_cast(angleAxis(m_horizontalAngle, vec3( 1.f, 0.f, 0.f)));

    //const mat4 zoom = scale(vec3(m_zoom, m_zoom, 1.f));
    const mat4 t    = translate(-m_position);

    const mat4 T1 = translate(vec3(0.f, 0.f, -512.f));

    m_axonometric = T1 * hrot * vrot * t; // * m_rotation;
    m_rebuild = false;
}

const mat4 & AxonometricLookAt::matrix() const
{
    if (m_rebuild)
        rebuild();

    return m_axonometric;
}

} // namespace glowutils
