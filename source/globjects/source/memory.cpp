#include <globjects/memory.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>

namespace {

gl::GLint getMemoryInformation(gl::GLenum pname)
{
    if (!globjects::hasExtension(gl::GLextension::GL_NVX_gpu_memory_info))
        return -1;

    return globjects::getInteger(pname);
}

}

namespace globjects
{

namespace memory
{

gl::GLint total()
{
    return getMemoryInformation(gl::GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX);
}

gl::GLint dedicated()
{
    return getMemoryInformation(gl::GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
}

gl::GLint available()
{
    return getMemoryInformation(gl::GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
}

gl::GLint evicted()
{
    return getMemoryInformation(gl::GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX);
}

gl::GLint evictionCount()
{
    return getMemoryInformation(gl::GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX);
}

} // namespace memory
} // namespace globjects
