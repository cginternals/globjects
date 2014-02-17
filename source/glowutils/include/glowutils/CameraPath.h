#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <glowutils/glowutils.h>


namespace glowutils
{

class Camera;

struct GLOWUTILS_API CameraPathPoint
{
public:
    CameraPathPoint();
    CameraPathPoint(const Camera& camera);
    CameraPathPoint(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up, float fov);

    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
    float fov;
};

class GLOWUTILS_API CameraPath
{
public:
    CameraPath();

    void append(const CameraPathPoint& point);

    CameraPath & operator<<(const CameraPathPoint& point);

    const std::vector<CameraPathPoint>& points() const;
protected:
    std::vector<CameraPathPoint> m_points;
};

} // namespace glowutils
