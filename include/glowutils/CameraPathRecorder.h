#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include <glow/ref_ptr.h>
#include <glowutils/glowutils.h>

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
