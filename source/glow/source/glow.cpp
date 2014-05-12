#include <glow/glow.h>

#include <glbinding/glbinding.h>
#include <glbinding/functions.h>
#include <glbinding/AbstractFunction.h>

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
    gl::AbstractFunction::enableCallbacksForAllExcept({ "glGetError" });

    gl::AbstractFunction::setAfterCallback([](const gl::AbstractFunction & function) {
        Error::check(function.name());
    });

    /*AbstractFunction::setBeforeCallback([](const AbstractFunction & f) {
        // logging
        std::cout << f.name() << std::endl;
    });*/

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
    const gl::GLubyte* result = gl::GetString(pname);

	return reinterpret_cast<const char*>(result);
}

std::string getString(gl::GLenum pname, gl::GLuint index)
{
    const gl::GLubyte* result = gl::GetStringi(pname, index);

    return reinterpret_cast<const char*>(result);
}

gl::GLint getInteger(gl::GLenum pname)
{
	gl::GLint value;

    gl::GetIntegerv(pname, &value);

	return value;
}

gl::GLenum getEnum(gl::GLenum pname)
{
    return static_cast<gl::GLenum>(getInteger(pname));
}

gl::GLfloat getFloat(gl::GLenum pname)
{
	gl::GLfloat value;

    gl::GetFloatv(pname, &value);

	return value;
}

gl::GLdouble getDouble(gl::GLenum pname)
{
	gl::GLdouble value;

    gl::GetDoublev(pname, &value);

	return value;
}

gl::GLboolean getBoolean(gl::GLenum pname)
{
	gl::GLboolean value;

    gl::GetBooleanv(pname, &value);

	return value;
}

gl::GLint getInteger(gl::GLenum pname, gl::GLuint index)
{
	gl::GLint value;

    gl::GetIntegeri_v(pname, index, &value);

	return value;
}

gl::GLenum getEnum(gl::GLenum pname, gl::GLuint index)
{
    return static_cast<gl::GLenum>(getInteger(pname, index));
}

gl::GLfloat getFloat(gl::GLenum pname, gl::GLuint index)
{
    gl::GLfloat value;

    gl::GetFloati_v(pname, index, &value);

    return value;
}

gl::GLdouble getDouble(gl::GLenum pname, gl::GLuint index)
{
    gl::GLdouble value;

    gl::GetDoublei_v(pname, index, &value);

    return value;
}

gl::GLboolean getBoolean(gl::GLenum pname, gl::GLuint index)
{
    gl::GLboolean value;

    gl::GetBooleani_v(pname, index, &value);

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

gl::GLint majorVersion()
{
    return getInteger(gl::MAJOR_VERSION);
}

gl::GLint minorVersion()
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
    gl::Enable(capability);
}

void disable(gl::GLenum capability)
{
    gl::Disable(capability);
}

bool isEnabled(gl::GLenum capability)
{
    gl::GLboolean value = gl::IsEnabled(capability);

    return value == gl::TRUE_;
}

void setEnabled(gl::GLenum capability, bool enabled)
{
    enabled ? enable(capability) : disable(capability);
}

void enable(gl::GLenum capability, int index)
{
    gl::Enablei(capability, index);
}

void disable(gl::GLenum capability, int index)
{
    gl::Disablei(capability, index);
}

bool isEnabled(gl::GLenum capability, int index)
{
    gl::GLboolean value = gl::IsEnabledi(capability, index);

    return value == gl::TRUE_;
}

void setEnabled(gl::GLenum capability, int index, bool enabled)
{
    enabled ? enable(capability, index) : disable(capability, index);
}

} // namespace glow
