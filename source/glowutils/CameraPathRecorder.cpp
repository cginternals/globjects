#include <glowutils/CameraPathRecorder.h>

#include <glowutils/Camera.h>

namespace glowutils
{

CameraPathRecorder::CameraPathRecorder(Camera & camera)
: m_camera(camera)
{
}

void CameraPathRecorder::record()
{
    m_path.append(CameraPathPoint(m_camera));
}

} // namespace glowutils
