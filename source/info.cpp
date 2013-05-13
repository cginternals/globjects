#ifdef WIN32
#include <GL/wglew.h>
#else
#include <GL/glxew.h>
#endif

#ifndef GL_VERSION_3_0
#define GL_NUM_EXTENSIONS						0x821D
#define GL_CONTEXT_FLAGS						0x821E
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT	0x0001
#endif

#ifndef GL_VERSION_3_2
#define GL_CONTEXT_CORE_PROFILE_BIT			0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT	0x00000002
#define GL_CONTEXT_PROFILE_MASK			0x9126
#endif

// http://developer.download.nvidia.com/opengl/specs/GL_NVX_gpu_memory_info.txt
#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX			0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX		0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX	0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX				0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX				0x904B


#include <glow/info.h>
#include <sstream>

using namespace glow;

// info

info::Version::Version(GLint majorVersion, GLint minorVersion)
: majorVersion(majorVersion)
, minorVersion(minorVersion)
{
}

bool info::Version::operator<(const Version& version) const
{
	return majorVersion<version.majorVersion || (majorVersion==version.majorVersion && minorVersion<version.minorVersion);
}

bool info::Version::operator>(const Version& version) const
{
	return majorVersion>version.majorVersion || (majorVersion==version.majorVersion && minorVersion>version.minorVersion);
}

bool info::Version::operator==(const Version& version) const
{
	return majorVersion==version.majorVersion && minorVersion==version.minorVersion;
}

bool info::Version::operator!=(const Version& version) const
{
	return majorVersion!=version.majorVersion || minorVersion!=version.minorVersion;
}

bool info::Version::operator>=(const Version& version) const
{
	return *this>version || *this==version;
}

bool info::Version::operator<=(const Version& version) const
{
	return *this<version || *this==version;
}

std::string info::Version::toString() const
{
	std::stringstream stream;
	stream << majorVersion << "." << minorVersion;
	return stream.str();
}

GLint info::getInteger(GLenum pname)
{
	GLint i;
	glGetIntegerv(pname, &i);
	return i;
}

std::string info::getString(GLenum pname)
{
	return reinterpret_cast<const char*>(glGetString(pname));
}

std::string info::vendor()
{
	return getString(GL_VENDOR);
}

std::string info::renderer()
{
	return getString(GL_RENDERER);
}

std::string info::versionString()
{
	return getString(GL_VERSION);
}

info::Version info::version()
{
	return Version(majorVersion(), minorVersion());
}

GLint info::majorVersion()
{
	return getInteger(GL_MAJOR_VERSION);
}

GLint info::minorVersion()
{
	return getInteger(GL_MINOR_VERSION);
}

bool info::isCoreProfile()
{
	if (version()<Version(3,2))
	{
		return false;
	}

	return (getInteger(GL_CONTEXT_PROFILE_MASK) & GL_CONTEXT_CORE_PROFILE_BIT) != 0;
}

// extensions

bool extensions::isSupported(const char* extension)
{
	if (info::isCoreProfile())
	{
		return false;
	}

	bool supported = glewIsSupported(extension);

	#ifdef WIN32
	supported |= wglewIsSupported(extension);
	#elif defined(LINUX)
	supported |= glxewIsSupported(extension);
	#endif

	return supported;
}

bool extensions::isSupported(const std::string& extension)
{
	return isSupported(extension.c_str());
}

// memory


GLint memory::total()
{
	return memoryInfo(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX);
}

GLint memory::dedicated()
{
	return memoryInfo(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
}

GLint memory::available()
{
	return memoryInfo(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
}

GLint memory::evicted()
{
	return memoryInfo(GPU_MEMORY_INFO_EVICTED_MEMORY_NVX);
}

GLint memory::evictionCount()
{
	return memoryInfo(GPU_MEMORY_INFO_EVICTION_COUNT_NVX);
}

GLint memory::memoryInfo(GLenum pname)
{
	if (!extensions::isSupported("GL_NVX_gpu_memory_info"))
	{
		return -1;
	}

	return info::getInteger(pname);
}

// glew

std::string glew::getString(GLenum pname)
{
	return reinterpret_cast<const char*>(glewGetString(pname));
}

std::string glew::version()
{
	return getString(GLEW_VERSION);
}

////const QString GL::glewError(const GLenum pname)
////{
////    const QString result = reinterpret_cast<const char*>(glewGetErrorString(pname));
////    return result;
////}

