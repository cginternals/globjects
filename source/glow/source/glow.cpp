#include <glow/glow.h>

#include <glbinding/glbinding.h>
#include <glbinding/functions.h>
#include <glbinding/AbstractFunction.h>
#include <glbinding/initialize.h>

#include <glow/Error.h>
#include <glow/logging.h>

#include "registry/Registry.h"
#include "registry/ExtensionRegistry.h"

#include <glow/NamedString.h>

namespace glow
{

bool glowIsInitialized = false;

bool initializeGLBinding()
{
    gl::AbstractFunction::setCallbackLevelForAllExcept(gl::AbstractFunction::CallbackLevel::After, { "glGetError" });

    gl::AbstractFunction::setAfterCallback([](const gl::AbstractFunction & function) {
        Error::check(function.name());
    });

    return gl::initialize();
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

    if (!initializeGLBinding())
    {
        return false;
    }

    Registry::registerCurrentContext();

    glowIsInitialized = true;

    return true;
}

std::string getString(gl::GLenum pname)
{
    const gl::GLubyte* result = gl::glGetString(pname);

	return reinterpret_cast<const char*>(result);
}

std::string getString(gl::GLenum pname, gl::GLuint index)
{
    const gl::GLubyte* result = gl::glGetStringi(pname, index);

    return reinterpret_cast<const char*>(result);
}

gl::GLint getInteger(gl::GLenum pname)
{
	gl::GLint value;

    gl::glGetIntegerv(pname, &value);

	return value;
}

gl::GLenum getEnum(gl::GLenum pname)
{
    return static_cast<gl::GLenum>(getInteger(pname));
}

gl::GLfloat getFloat(gl::GLenum pname)
{
	gl::GLfloat value;

    gl::glGetFloatv(pname, &value);

	return value;
}

gl::GLdouble getDouble(gl::GLenum pname)
{
	gl::GLdouble value;

    gl::glGetDoublev(pname, &value);

	return value;
}

gl::GLboolean getBoolean(gl::GLenum pname)
{
	gl::GLboolean value;

    gl::glGetBooleanv(pname, &value);

	return value;
}

gl::GLint getInteger(gl::GLenum pname, gl::GLuint index)
{
	gl::GLint value;

    gl::glGetIntegeri_v(pname, index, &value);

	return value;
}

gl::GLenum getEnum(gl::GLenum pname, gl::GLuint index)
{
    return static_cast<gl::GLenum>(getInteger(pname, index));
}

gl::GLfloat getFloat(gl::GLenum pname, gl::GLuint index)
{
    gl::GLfloat value;

    gl::glGetFloati_v(pname, index, &value);

    return value;
}

gl::GLdouble getDouble(gl::GLenum pname, gl::GLuint index)
{
    gl::GLdouble value;

    gl::glGetDoublei_v(pname, index, &value);

    return value;
}

gl::GLboolean getBoolean(gl::GLenum pname, gl::GLuint index)
{
    gl::GLboolean value;

    gl::glGetBooleani_v(pname, index, &value);

    return value;
}

std::string vendor()
{
    return getString(gl::GL_VENDOR);
}

std::string renderer()
{
    return getString(gl::GL_RENDERER);
}

std::string versionString()
{
    return getString(gl::GL_VERSION);
}

gl::GLint majorVersion()
{
    return getInteger(gl::GL_MAJOR_VERSION);
}

gl::GLint minorVersion()
{
    return getInteger(gl::GL_MINOR_VERSION);
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

    return (getInteger(gl::GL_CONTEXT_PROFILE_MASK) & gl::GL_CONTEXT_CORE_PROFILE_BIT) > 0;
}

std::vector<std::string> getExtensions()
{
    int count = getInteger(gl::GL_NUM_EXTENSIONS);

    std::vector<std::string> extensions(count);

    for (int i=0; i<count; ++i)
    {
        extensions[i] = getString(gl::GL_EXTENSIONS, i);
    }

    return extensions;
}

bool hasExtension(gl::Extension extension)
{
    return ExtensionRegistry::current().hasExtension(extension);
}

bool hasExtension(const std::string & extensionName)
{
    return ExtensionRegistry::current().hasExtension(extensionName);
}

bool isInCoreProfile(gl::Extension extension, const Version & version)
{
    return ExtensionRegistry::current().isInCoreProfile(extension, version);
}

bool isInCoreProfile(gl::Extension extension)
{
    return ExtensionRegistry::current().isInCoreProfile(extension);
}

void enable(gl::GLenum capability)
{
    gl::glEnable(capability);
}

void disable(gl::GLenum capability)
{
    gl::glDisable(capability);
}

bool isEnabled(gl::GLenum capability)
{
    gl::GLboolean value = gl::glIsEnabled(capability);

    return value == gl::GL_TRUE;
}

void setEnabled(gl::GLenum capability, bool enabled)
{
    enabled ? enable(capability) : disable(capability);
}

void enable(gl::GLenum capability, int index)
{
    gl::glEnablei(capability, index);
}

void disable(gl::GLenum capability, int index)
{
    gl::glDisablei(capability, index);
}

bool isEnabled(gl::GLenum capability, int index)
{
    gl::GLboolean value = gl::glIsEnabledi(capability, index);

    return value == gl::GL_TRUE;
}

void setEnabled(gl::GLenum capability, int index, bool enabled)
{
    enabled ? enable(capability, index) : disable(capability, index);
}

} // namespace glow
