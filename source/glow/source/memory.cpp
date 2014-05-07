#include <glow/memory.h>

#include <glbinding/constants.h>

#include <glow/glow.h>
#include <glow/Extension.h>

namespace {

GLint getMemoryInformation(GLenum pname)
{
    if (!glow::hasExtension("NVX_gpu_memory_info"))
        return -1;

    return glow::getInteger(pname);
}

}

namespace glow
{

namespace memory
{

GLint total()
{
    return getMemoryInformation(gl::GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX);
}

GLint dedicated()
{
    return getMemoryInformation(gl::GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
}

GLint available()
{
    return getMemoryInformation(gl::GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
}

GLint evicted()
{
    return getMemoryInformation(gl::GPU_MEMORY_INFO_EVICTED_MEMORY_NVX);
}

GLint evictionCount()
{
    return getMemoryInformation(gl::GPU_MEMORY_INFO_EVICTION_COUNT_NVX);
}

}
}
