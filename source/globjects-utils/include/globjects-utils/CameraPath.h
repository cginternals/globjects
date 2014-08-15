#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <globjects-utils/globjects-utils_api.h>


namespace gloutils
{

class Camera;

struct GLOBJECTS_UTILS_API CameraPathPoint
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

class GLOBJECTS_UTILS_API CameraPath
{
public:
    CameraPath();

    void append(const CameraPathPoint& point);

    CameraPath & operator<<(const CameraPathPoint& point);

    const std::vector<CameraPathPoint>& points() const;
protected:
    std::vector<CameraPathPoint> m_points;
};

} // namespace gloutils
