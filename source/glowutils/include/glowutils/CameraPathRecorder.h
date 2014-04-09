#pragma once

#include <glowutils/glowutils_api.h>

#include <glowutils/CameraPath.h>

namespace glowutils
{
class Camera;

class GLOWUTILS_API CameraPathRecorder
{
public:
    CameraPathRecorder(Camera & camera);

    void record();
protected:
    Camera& m_camera;
    CameraPath m_path;
};

} // namespace glowutils
