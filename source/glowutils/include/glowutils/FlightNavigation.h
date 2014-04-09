#pragma once

#include <glm/glm.hpp>

#include <glowutils/glowutils_api.h>

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
    enum Direction
    {
        None = 0x0,
        Left = 0x1,
        Right = 0x2,
        Forward = 0x4,
        Backward = 0x8,
        All = Left | Right | Forward | Backward
    };

    void mouseMove(const glm::ivec2 & mouseDelta);

    void startMovement(unsigned direction);
    void stopMovement(unsigned direction);
    bool isMoving(Direction direction) const;

    void move(float delta);

protected:
    Camera * m_camera;
    float m_mouseSpeed;
    float m_moveSpeed;
    unsigned m_directions;

    void move(const glm::vec3 & direction, float delta);
};

} // namespace glowutils
