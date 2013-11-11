#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include <glowutils/glowutils.h>



namespace glow
{

class Camera;


struct GLOWUTILS_API CameraPathPoint
{
public:
    static CameraPathPoint fromCamera(const Camera& camera);

    CameraPathPoint();
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

    const std::vector<CameraPathPoint>& points() const;
protected:
    std::vector<CameraPathPoint> m_points;
};

class GLOWUTILS_API CameraPathRecorder
{
public:
    CameraPathRecorder(Camera & camera);

    void record();
protected:
    Camera& m_camera;
    CameraPath m_path;
};

class GLOWUTILS_API CameraPathPlayer
{
public:
    CameraPathPlayer(Camera & camera, const CameraPath & path);

    void play(float t);
protected:
    Camera& m_camera;
    CameraPath m_path;
    std::map<float, CameraPathPoint> m_ranges;
    float m_total;

    void prepare();
    void find(float t, CameraPathPoint& p1, CameraPathPoint& p2, float& localT);
};

} // namespace glow
