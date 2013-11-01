
#include <GL/glew.h>

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

#include <glow/global.h>

#include <glow/Error.h>


namespace glow
{


// query

std::string query::getString(GLenum pname)
{
	const GLubyte* result = glGetString(pname);
	CheckGLError();
	return reinterpret_cast<const char*>(result);
}

GLint query::getInteger(GLenum pname)
{
	GLint value;

	glGetIntegerv(pname, &value);
	CheckGLError();

	return value;
}

GLfloat query::getFloat(GLenum pname)
{
	GLfloat value;

	glGetFloatv(pname, &value);
	CheckGLError();

	return value;
}

GLdouble query::getDouble(GLenum pname)
{
	GLdouble value;

	glGetDoublev(pname, &value);
	CheckGLError();

	return value;
}

GLboolean query::getBoolean(GLenum pname)
{
	GLboolean value;

	glGetBooleanv(pname, &value);
	CheckGLError();

	return value;
}

GLint query::getInteger(GLenum pname, unsigned index)
{
	GLint value;

	glGetIntegeri_v(pname, index, &value);
	CheckGLError();

	return value;
}

std::vector<GLint> query::getIntegers(GLenum pname, unsigned size)
{
	GLint* values = new GLint[size];

	glGetIntegerv(pname, values);
	CheckGLError();

	std::vector<GLint> result(values, values+size);
	delete[] values;

	return result;
}

std::string query::vendor()
{
	return getString(GL_VENDOR);
}

std::string query::renderer()
{
	return getString(GL_RENDERER);
}

std::string query::versionString()
{
	return getString(GL_VERSION);
}

Version query::version()
{
	return Version(majorVersion(), minorVersion());
}

GLint query::majorVersion()
{
	return getInteger(GL_MAJOR_VERSION);
}

GLint query::minorVersion()
{
	return getInteger(GL_MINOR_VERSION);
}

bool query::isCoreProfile()
{
	if (version()<Version(3,2))
	{
		return false;
	}

	return (getInteger(GL_CONTEXT_PROFILE_MASK) & GL_CONTEXT_CORE_PROFILE_BIT) != 0;
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
    if (!GLEW_NVX_gpu_memory_info)
		return -1;

	return query::getInteger(pname);
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

} // namespace glow
