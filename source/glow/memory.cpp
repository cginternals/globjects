#include <glow/memory.h>

#include <glow/global.h>

namespace glow
{

namespace memory
{

GLint total()
{
    return get(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX);
}

GLint dedicated()
{
    return get(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
}

GLint available()
{
    return get(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
}

GLint evicted()
{
    return get(GPU_MEMORY_INFO_EVICTED_MEMORY_NVX);
}

GLint evictionCount()
{
    return get(GPU_MEMORY_INFO_EVICTION_COUNT_NVX);
}

GLint get(GLenum pname)
{
    if (!GLEW_NVX_gpu_memory_info)
        return -1;

    return getInteger(pname);
}

}
}
