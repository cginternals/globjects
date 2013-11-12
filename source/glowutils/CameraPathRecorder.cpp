
#include <cassert>
#include <string>
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

#include <glowutils/Camera.h>
#include <glowutils/CameraPathRecorder.h>


using namespace glm;

namespace glow
{


CameraPathPoint CameraPathPoint::fromCamera(const Camera& camera)
{
    return CameraPathPoint(camera.eye(), camera.center(), camera.up(), camera.fovy());
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
//    memcpy(this->eye, glm::value_ptr(eye), 3*sizeof(float));
//    memcpy(this->center, glm::value_ptr(center), 3*sizeof(float));
//    memcpy(this->up, glm::value_ptr(up), 3*sizeof(float));
}

CameraPath::CameraPath()
{
}

void CameraPath::append(const CameraPathPoint& point)
{
    m_points.push_back(point);
}

const std::vector<CameraPathPoint>& CameraPath::points() const
{
    return m_points;
}

CameraPathRecorder::CameraPathRecorder(Camera & camera)
: m_camera(camera)
{
}

void CameraPathRecorder::record()
{
    m_path.append(CameraPathPoint::fromCamera(m_camera));
}

CameraPathPlayer::CameraPathPlayer(Camera & camera, const CameraPath & path)
: m_camera(camera)
, m_path(path)
{
    prepare();
}

void CameraPathPlayer::prepare()
{
    m_total = 0.0;
    const std::vector<CameraPathPoint>& points = m_path.points();

    if (points.size()<=1)
        return;

    CameraPathPoint previous = points[0];
    m_ranges[0.0f] = previous;

    for (int i = 1; i<points.size(); ++i)
    {
        CameraPathPoint current = points[i];

        float d = glm::length(current.eye - previous.eye);

        m_total += d;
        m_ranges[m_total] = current;

        previous = current;
    }
}

void CameraPathPlayer::find(const float t, CameraPathPoint& p1, CameraPathPoint& p2, float& localT)
{
    float absoluteT = m_total * glm::clamp(t, 0.0f, 1.0f);

    typedef std::pair<float, CameraPathPoint> P;
    auto found = std::lower_bound(m_ranges.begin(), m_ranges.end(), absoluteT, [](const P& p, float f) {
        return p.first<f;
    });

    if (found == m_ranges.cend())
    {
        assert(false);
        return;
    }

    auto next = found;
    ++next;

    localT = (absoluteT - found->first) / (next->first - found->first);
    p1 = found->second;
    p2 = next->second;
}

void CameraPathPlayer::play(const float t)
{
    CameraPathPoint p1;
    CameraPathPoint p2;
    float localT;
    find(t, p1, p2, localT);
}

} // namespace glow
