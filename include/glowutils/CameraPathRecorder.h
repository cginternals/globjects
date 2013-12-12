#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include <glow/ref_ptr.h>
#include <glowutils/glowutils.h>



namespace glowutils
{
class Camera;
class VertexArrayObject;
class Program;
class Buffer;


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
    CameraPathPlayer(Camera & camera);

    void setPath(const CameraPath & path);

    void play(float t);

    void createVao();
    void draw(const glm::mat4& viewProjection);
protected:
    struct PathSection
    {
        const CameraPathPoint* start;
        const CameraPathPoint* end;
        float startT;
        float endT;

        glm::vec3 c1;
        glm::vec3 c2;
    };

    Camera& m_camera;
    CameraPath m_path;
    std::vector<PathSection> m_sections;
    ref_ptr<VertexArrayObject> m_vao;
    ref_ptr<Program> m_program;
    ref_ptr<Buffer> m_buffer;
    int m_bufferSize;

    void prepare();
    void prepareControlPoints();

    PathSection& find(float t);
    CameraPathPoint interpolate(const PathSection& section, float t);
    void moveCamera(const CameraPathPoint& point);
};

} // namespace glowutils
