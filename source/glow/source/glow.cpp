#include <glow/glow.h>

#include <glow/Error.h>
#include <glow/logging.h>

#include "registry/Registry.h"
#include "registry/ExtensionRegistry.h"

#include <glow/NamedString.h>

namespace glow
{

bool glowIsInitialized = false;

bool initializeGLEW(bool showWarnings)
{
    glewExperimental = gl::TRUE;

    GLenum result = glewInit();
    if (result != GLEW_OK)
    {
        if (showWarnings)
        {
            warning() << reinterpret_cast<const char*>(glewGetErrorString(result));
        }

        return false;
    }

    // NOTE: should be safe to ignore:
    // http://www.opengl.org/wiki/Opengl::Loading_Library
    Error::clear(); // ignore gl::INVALID_ENUM

    return true;
}

bool isInitialized()
{
    return glowIsInitialized;
}

bool init(bool showWarnings)
{
    if (glowIsInitialized)
    {
        if (showWarnings)
        {
            warning() << "glow is already initialized";
        }

        return true;
    }

    if (!initializeGLEW(showWarnings))
    {
        return false;
    }

    Registry::registerCurrentContext();

    glowIsInitialized = true;

    return true;
}

std::string getString(GLenum pname)
{
	const GLubyte* result = glGetString(pname);
	CheckGLError();
	return reinterpret_cast<const char*>(result);
}

std::string getString(GLenum pname, GLuint index)
{
    const GLubyte* result = glGetStringi(pname, index);
    CheckGLError();
    return reinterpret_cast<const char*>(result);
}

GLint getInteger(GLenum pname)
{
	GLint value;

	glGetIntegerv(pname, &value);
	CheckGLError();

	return value;
}

GLenum getEnum(GLenum pname)
{
    return static_cast<GLenum>(getInteger(pname));
}

GLfloat getFloat(GLenum pname)
{
	GLfloat value;

	glGetFloatv(pname, &value);
	CheckGLError();

	return value;
}

GLdouble getDouble(GLenum pname)
{
	GLdouble value;

	glGetDoublev(pname, &value);
	CheckGLError();

	return value;
}

GLboolean getBoolean(GLenum pname)
{
	GLboolean value;

	glGetBooleanv(pname, &value);
	CheckGLError();

	return value;
}

GLint getInteger(GLenum pname, GLuint index)
{
	GLint value;

	glGetIntegeri_v(pname, index, &value);
	CheckGLError();

	return value;
}

GLenum getEnum(GLenum pname, GLuint index)
{
    return static_cast<GLenum>(getInteger(pname, index));
}

GLfloat getFloat(GLenum pname, GLuint index)
{
    GLfloat value;

    glGetFloati_v(pname, index, &value);
    CheckGLError();

    return value;
}

GLdouble getDouble(GLenum pname, GLuint index)
{
    GLdouble value;

    glGetDoublei_v(pname, index, &value);
    CheckGLError();

    return value;
}

GLboolean getBoolean(GLenum pname, GLuint index)
{
    GLboolean value;

    glGetBooleani_v(pname, index, &value);
    CheckGLError();

    return value;
}

std::string vendor()
{
    return getString(gl::VENDOR);
}

std::string renderer()
{
    return getString(gl::RENDERER);
}

std::string versionString()
{
    return getString(gl::VERSION);
}

GLint majorVersion()
{
    return getInteger(gl::MAJOR_VERSION);
}

GLint minorVersion()
{
    return getInteger(gl::MINOR_VERSION);
}

Version version()
{
    return Version(majorVersion(), minorVersion());
}

bool isCoreProfile()
{
    if (version()<Version(3,2))
    {
        return false;
    }

    return (getInteger(gl::CONTEXT_PROFILE_MASK) & gl::CONTEXT_CORE_PROFILE_BIT) > 0;
}

std::vector<std::string> getExtensions()
{
    int count = getInteger(gl::NUM_EXTENSIONS);

    std::vector<std::string> extensions(count);

    for (int i=0; i<count; ++i)
    {
        extensions[i] = getString(gl::EXTENSIONS, i);
    }

    return extensions;
}

bool hasExtension(Extension extension)
{
    return ExtensionRegistry::current().hasExtension(extension);
}

bool hasExtension(const std::string & extensionName)
{
    return ExtensionRegistry::current().hasExtension(extensionName);
}

bool isInCoreProfile(Extension extension, const Version & version)
{
    return ExtensionRegistry::current().isInCoreProfile(extension, version);
}

bool isInCoreProfile(Extension extension)
{
    return ExtensionRegistry::current().isInCoreProfile(extension);
}

void enable(GLenum capability)
{
    glEnable(capability);
    CheckGLError();
}

void disable(GLenum capability)
{
    glDisable(capability);
    CheckGLError();
}

bool isEnabled(GLenum capability)
{
    GLboolean value = glIsEnabled(capability);
    CheckGLError();
    return value == gl::TRUE;
}

void setEnabled(GLenum capability, bool enabled)
{
    enabled ? enable(capability) : disable(capability);
}

void enable(GLenum capability, int index)
{
    glEnablei(capability, index);
    CheckGLError();
}

void disable(GLenum capability, int index)
{
    glDisablei(capability, index);
    CheckGLError();
}

bool isEnabled(GLenum capability, int index)
{
    GLboolean value = glIsEnabledi(capability, index);
    CheckGLError();
    return value == gl::TRUE;
}

void setEnabled(GLenum capability, int index, bool enabled)
{
    enabled ? enable(capability, index) : disable(capability, index);
}

} // namespace glow
