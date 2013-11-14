
#include <cassert>
#include <string>
#include <algorithm>
#include <limits>

#include <glm/gtc/type_ptr.hpp>

#include <glowutils/Camera.h>
#include <glowutils/CameraPathRecorder.h>

#include <glow/logging.h>

using namespace glm;

namespace glow
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

CameraPath & CameraPath::operator<<(const CameraPathPoint& point)
{
    append(point);
    return *this;
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
    m_path.append(CameraPathPoint(m_camera));
}

CameraPathPlayer::CameraPathPlayer(Camera & camera)
: m_camera(camera)
{
}

void CameraPathPlayer::setPath(const CameraPath & path)
{
   m_path = path;
   prepare();
}

void CameraPathPlayer::prepare()
{
    m_total = 0.0;
    const std::vector<CameraPathPoint>& points = m_path.points();

    if (points.size()<=1)
        return;

    CameraPathPoint previous = points[0];
    m_ranges[-1] = previous; // has to be less than -0 or else lower_bound will return first element for 0, not second

    for (int i = 1; i<points.size(); ++i)
    {
        CameraPathPoint current = points[i];

        float d = glm::length(current.eye - previous.eye);

        m_total += d;
        m_ranges[m_total] = current;

        previous = current;
    }

    // -----------------------
    CameraPathPoint previous = points[0];
    CameraPathPoint current = points[1];

    vec3 dir0 = current.eye - previous.eye;

    for (int i = 1; i<points.size()-1; ++i)
    {
        CameraPathPoint current = points[i];
        CameraPathPoint next = points[i+1];

        vec3 t1 = current.eye - previous.eye;
        vec3 t2 = next.eye - current.eye;

        vec3 dirN = (t1+t2)/2.0;

        previous = current;
    }

    CameraPathPoint last = points[points.size()-1];
    vec3 dirLast = last.eye - previous.eye;
}

void CameraPathPlayer::find(const float t, CameraPathPoint& p1, CameraPathPoint& p2, float& localT)
{
    float absoluteT = m_total * glm::clamp(t, 0.0f, 1.0f);

    assert(absoluteT >= 0);
    assert(absoluteT <= m_total);

    typedef std::pair<float, CameraPathPoint> P;

    auto found1 = std::lower_bound(m_ranges.begin(), m_ranges.end(), absoluteT, [](const P& p, float f) {
        return p.first<f;
    });

    assert(found1 != m_ranges.cend());

    auto found = std::map<float, CameraPathPoint>::reverse_iterator(found1);

    if (found == m_ranges.rend())
    {
        assert(false);
        return;
    }    

    auto next = found1;

    p1 = found->second;
    p2 = next->second;

    assert(absoluteT >= found->first);
    assert(absoluteT <= next->first);

    localT = (absoluteT - found->first) / (next->first - found->first);
}

CameraPathPoint CameraPathPlayer::interpolate(const CameraPathPoint& p1, const CameraPathPoint& p2, float t)
{
    return CameraPathPoint(
                p1.eye*(1.0f-t) + p2.eye*t,
                p1.center*(1.0f-t) + p2.center*t,
                p1.up*(1.0f-t) + p2.up*t,
                p1.fov*(1.0f-t) + p2.fov*t);
}

void CameraPathPlayer::play(const float t)
{
    CameraPathPoint p1;
    CameraPathPoint p2;
    float localT;
    find(t, p1, p2, localT);

    CameraPathPoint p = interpolate(p1, p2, localT);

    m_camera.setCenter(p.center);
    m_camera.setEye(p.eye);
    m_camera.setUp(p.up);
    m_camera.setFovy(p.fov);
}

} // namespace glow
