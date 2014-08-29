#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include <globjects-base/ref_ptr.h>

#include <globjects/VertexArray.h>
#include <globjects/Program.h>
#include <globjects/Buffer.h>

#include <globjects-utils/globjects-utils_api.h>
#include <globjects-utils/CameraPath.h>


namespace gloutils
{

class Camera;

class GLOBJECTS_UTILS_API CameraPathPlayer
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
    glo::ref_ptr<glo::VertexArray> m_vao;
    glo::ref_ptr<glo::Program> m_program;
    glo::ref_ptr<glo::Buffer> m_buffer;
    int m_bufferSize;

    void prepare();
    void prepareControlPoints();

    PathSection& find(float t);
    CameraPathPoint interpolate(const PathSection& section, float t);
    void moveCamera(const CameraPathPoint& point);
};

} // namespace gloutils
