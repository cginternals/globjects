
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
    const std::vector<CameraPathPoint>& points = m_path.points();

    if (points.size()<=1)
        return;

    float totalLength = 0.0;
    const CameraPathPoint* previous = &points[0];

    for (int i = 1; i<points.size(); ++i)
    {
        const CameraPathPoint* current = &points[i];
        float distance = glm::length(current->eye - previous->eye);

        float startT = totalLength;
        float endT = startT + distance;

        PathSection section{ previous, current, startT, endT };
        m_sections.push_back(section);

        totalLength = endT;
        previous = current;
    }

    // normalize
    for (PathSection& section : m_sections)
    {
        section.startT /= totalLength;
        section.endT /= totalLength;
    }

    // -----------------------
    /*previous = points[0];
    CameraPathPoint current = points[1];


    std::vector<glm::vec3> m_dirs;
    vec3 dir0 = current.eye - previous.eye;

    m_dirs[0] = dir0;

    for (int i = 1; i<points.size()-1; ++i)
    {
        CameraPathPoint current = points[i];
        CameraPathPoint next = points[i+1];

        vec3 t1 = current.eye - previous.eye;
        vec3 t2 = next.eye - current.eye;

        vec3 dirN = (t1+t2)/2.0f;
        m_dirs[i] = dirN;

        previous = current;
    }

    CameraPathPoint last = points[points.size()-1];
    vec3 dirLast = last.eye - previous.eye;
    m_dirs[points.size()-1] = dirLast;*/


}

CameraPathPlayer::PathSection& CameraPathPlayer::find(const float t)
{
    return *std::lower_bound(m_sections.begin(), m_sections.end(), t, [](const PathSection& section, float value) {
        return section.endT < value;
    });
}

CameraPathPoint CameraPathPlayer::interpolate(const CameraPathPoint& p1, const CameraPathPoint& p2, const float t)
{
    return CameraPathPoint(
                p1.eye*(1.0f-t) + p2.eye*t,
                p1.center*(1.0f-t) + p2.center*t,
                p1.up*(1.0f-t) + p2.up*t,
                p1.fov*(1.0f-t) + p2.fov*t);
}

void CameraPathPlayer::moveCamera(const CameraPathPoint& point)
{
    m_camera.setCenter(point.center);
    m_camera.setEye(point.eye);
    m_camera.setUp(point.up);
    m_camera.setFovy(point.fov);
}

void CameraPathPlayer::play(const float t)
{
    const float clampedT = glm::clamp(t, 0.f, 1.f);

    PathSection& section = find(clampedT);
    const float sectionT = (clampedT - section.startT) / (section.endT - section.startT);

    moveCamera(interpolate(*section.start, *section.end, sectionT));
}

} // namespace glow
