
#include <glm/gtc/matrix_transform.hpp>

#include <glowutils/FlightNavigation.h>
#include <glowutils/Camera.h>

#include <glow/logging.h>

using namespace glm;

namespace glowutils
{

FlightNavigation::FlightNavigation()
: m_camera(nullptr)
, m_mouseSpeed(20.0)
, m_moveSpeed(0.2)
{
}

FlightNavigation::~FlightNavigation()
{
}

void FlightNavigation::setCamera(Camera * camera)
{
    m_camera = camera;
}

void FlightNavigation::setMouseSpeed(float speed)
{
    m_mouseSpeed = speed;
}

void FlightNavigation::setMoveSpeed(float speed)
{
    m_moveSpeed = speed;
}

void FlightNavigation::mouseMove(const ivec2 & mouseDelta)
{
    ivec2 viewport = m_camera->viewport();

    float z = m_camera->zFar();

    vec3 oldScreenCoords = vec3(vec2(viewport)/2.f, z);
    vec3 newScreenCoords = vec3(vec2(viewport)/2.f+vec2(mouseDelta.x, -mouseDelta.y), z);

    vec3 oldWorldCoords = glm::unProject(oldScreenCoords, mat4(), m_camera->viewProjection(), vec4(vec2(), viewport));
    vec3 newWorldCoords = glm::unProject(newScreenCoords, mat4(), m_camera->viewProjection(), vec4(vec2(), viewport));

    vec3 from = oldWorldCoords - m_camera->center();
    vec3 to = newWorldCoords - m_camera->center();

    glm::quat rot(glm::dot(from, to), glm::cross(from, to));

    vec3 eyeDir = normalize(m_camera->center() - m_camera->eye());
    eyeDir = rot * eyeDir;

    vec3 delta = eyeDir * m_mouseSpeed;
    m_camera->setCenter(m_camera->eye()+delta);
}

void FlightNavigation::moveLeft()
{
    vec3 eyeDir = m_camera->center() - m_camera->eye();
    vec3 sideDir = cross(eyeDir, m_camera->up());
    move(-sideDir);
}

void FlightNavigation::moveRight()
{
    vec3 eyeDir = m_camera->center() - m_camera->eye();
    vec3 sideDir = cross(eyeDir, m_camera->up());
    move(sideDir);
}

void FlightNavigation::moveForward()
{
    vec3 eyeDir = m_camera->center() - m_camera->eye();
    move(eyeDir);
}

void FlightNavigation::moveBackward()
{
    vec3 eyeDir = m_camera->center() - m_camera->eye();
    move(-eyeDir);
}

void FlightNavigation::move(const glm::vec3 & direction)
{
    vec3 delta = normalize(direction) * m_moveSpeed;
    m_camera->setEye(m_camera->eye() + delta);
    m_camera->setCenter(m_camera->center() + delta);
}

} // namespace glowutils
