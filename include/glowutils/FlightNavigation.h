#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils.h>

namespace glowutils
{

class Camera;


class GLOWUTILS_API FlightNavigation
{
public:
    FlightNavigation();
    virtual ~FlightNavigation();

    void setCamera(Camera * camera);

    void setMouseSpeed(float speed);
    void setMoveSpeed(float speed);
public:
    void mouseMove(const glm::ivec2 & mouseDelta);
    void moveLeft();
    void moveRight();
    void moveForward();
    void moveBackward();

protected:
    Camera * m_camera;
    float m_mouseSpeed;
    float m_moveSpeed;

    void move(const glm::vec3 & direction);
};

} // namespace glowutils
