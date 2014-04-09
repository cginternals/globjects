#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include <glow/ref_ptr.h>
#include <glowutils/glowutils_api.h>

#include <glowutils/CameraPath.h>

namespace glow {

class VertexArrayObject;
class Program;
class Buffer;

}

namespace glowutils
{

class Camera;

class GLOWUTILS_API CameraPathPlayer
{
public:
    CameraPathPlayer(Camera & camera);

    void setPath(const CameraPath & path);

    void play(float t);

    void createVao();
    void draw(const glm::mat4& viewProjection);
    void freeVao();
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
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glow::Buffer> m_buffer;
    int m_bufferSize;

    void prepare();
    void prepareControlPoints();

    PathSection& find(float t);
    CameraPathPoint interpolate(const PathSection& section, float t);
    void moveCamera(const CameraPathPoint& point);
};

} // namespace glowutils
