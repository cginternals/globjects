#include <globjects/memory.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>


using namespace gl;

namespace 
{

GLint getMemoryInformation(const GLenum pname)
{
    if (!globjects::hasExtension(GLextension::GL_NVX_gpu_memory_info))
        return -1;

    return globjects::getInteger(pname);
}

}

namespace globjects
{

namespace memory
{

GLint total()
{
    return getMemoryInformation(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX);
}

GLint dedicated()
{
    return getMemoryInformation(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
}

GLint available()
{
    return getMemoryInformation(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
}

GLint evicted()
{
    return getMemoryInformation(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX);
}

GLint evictionCount()
{
    return getMemoryInformation(GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX);
}

} // namespace memory
} // namespace globjects
