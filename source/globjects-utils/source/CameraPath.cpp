#include <globjects-utils/CameraPath.h>

#include <glm/gtc/type_ptr.hpp>

#include <globjects-utils/Camera.h>

namespace gloutils
{

CameraPathPoint::CameraPathPoint(const Camera& camera)
: eye(camera.eye())
, center(camera.center())
, up(camera.up())
, fov(camera.fovy())
{
}

CameraPathPoint::CameraPathPoint()
: fov(0)
{
}

CameraPathPoint::CameraPathPoint(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up, float fov)
: eye(eye)
, center(center)
, up(up)
, fov(fov)
{
}

CameraPath::CameraPath()
{
}

void CameraPath::append(const CameraPathPoint& point)
{
    m_points.push_back(point);
}

CameraPath & CameraPath::operator<<(const CameraPathPoint& point)
{
    append(point);
    return *this;
}

const std::vector<CameraPathPoint>& CameraPath::points() const
{
    return m_points;
}

} // namespace gloutils
