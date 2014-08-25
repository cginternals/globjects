#pragma once

#include <globjects-utils/globjects-utils_api.h>

#include <globjects-utils/CameraPath.h>

namespace gloutils
{
class Camera;

class GLOBJECTS_UTILS_API CameraPathRecorder
{
public:
    CameraPathRecorder(Camera & camera);

    void record();
protected:
    Camera& m_camera;
    CameraPath m_path;
};

} // namespace gloutils
