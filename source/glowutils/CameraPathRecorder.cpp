
#include <cassert>
#include <string>
#include <algorithm>

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
, c('A')
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
    previous.c = 'A';
    m_ranges[-0.1f] = previous;

    for (int i = 1; i<points.size(); ++i)
    {
        CameraPathPoint current = points[i];

        current.c = 'A'+static_cast<char>(i);

        float d = glm::length(current.eye - previous.eye);

        m_total += d;
        m_ranges[m_total] = current;

        previous = current;
    }

    for (auto& pair : m_ranges)
    {
        char n[2] = {pair.second.c, '\0'};

        glow::debug() << pair.first << " -> " << (const char*)n;
    }

    glow::debug() << "------";
    //exit(1);
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

    char r1[2] = {found1->second.c, '\0'};
    glow::debug() << absoluteT << " : " << (const char*)r1;

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

    char n1[2] = {p1.c, '\0'};
    char n2[2] = {p2.c, '\0'};
    glow::debug() << t << " : " << localT << " " << (const char*)n1 << " - " << (const char*)n2;
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
