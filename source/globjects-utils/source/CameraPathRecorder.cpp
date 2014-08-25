#include <globjects-utils/CameraPathRecorder.h>

#include <globjects-utils/Camera.h>

namespace gloutils
{

CameraPathRecorder::CameraPathRecorder(Camera & camera)
: m_camera(camera)
{
}

void CameraPathRecorder::record()
{
    m_path.append(CameraPathPoint(m_camera));
}

} // namespace gloutils
