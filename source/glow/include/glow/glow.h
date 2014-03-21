#pragma once

// NOTE: don't export stl stuff (e.g. containers):
// http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
// don't do it: http://support.microsoft.com/kb/q168958/

#ifdef _MSC_VER
#	define GLOW_API_EXPORT_DECLARATION __declspec(dllexport)
#	define GLOW_API_IMPORT_DECLARATION __declspec(dllimport)
#elif __GNUC__
#	define GLOW_API_EXPORT_DECLARATION __attribute__ ((visibility ("default")))
#	define GLOW_API_IMPORT_DECLARATION __attribute__ ((visibility ("default")))
#else
#	define GLOW_API_EXPORT_DECLARATION
#	define GLOW_API_IMPORT_DECLARATION
#endif

#ifndef GLOW_STATIC
#ifdef GLOW_EXPORTS
#	define GLOW_API GLOW_API_EXPORT_DECLARATION
#else
#	define GLOW_API GLOW_API_IMPORT_DECLARATION
#endif
#else
#   define GLOW_API
#endif

#ifdef N_DEBUG
#	define IF_DEBUG(statement)
#	define IF_NDEBUG(statement) statement
#else
#	define IF_DEBUG(statement) statement
#	define IF_NDEBUG(statement)
#endif // N_DEBUG

// http://stackoverflow.com/questions/18387640/how-to-deal-with-noexcept-in-visual-studio
#ifndef NOEXCEPT
#	ifdef _MSC_VER
#		define NOEXCEPT
#	else
#		define NOEXCEPT noexcept
#	endif
#endif

#ifndef GL_VERSION_3_0
#define GL_NUM_EXTENSIONS                       0x821D
#define GL_CONTEXT_FLAGS                        0x821E
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT  0x0001
#endif

#ifndef GL_VERSION_3_2
#define GL_CONTEXT_CORE_PROFILE_BIT             0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT    0x00000002
#define GL_CONTEXT_PROFILE_MASK                 0x9126
#endif

// http://developer.download.nvidia.com/opengl/specs/GL_NVX_gpu_memory_info.txt
#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX            0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX      0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX    0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX              0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX              0x904B
