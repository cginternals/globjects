#include <glow/memory.h>

#include <glow/global.h>
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

}
}
