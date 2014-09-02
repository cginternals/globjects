#include <globjects/globjects.h>

#include <unordered_map>
#include <mutex>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/functions.h>
#include <glbinding/AbstractFunction.h>
#include <glbinding/Binding.h>
#include <glbinding/callbacks.h>

#include <globjects/Error.h>
#include <globjects/logging.h>

#include "registry/Registry.h"
#include "registry/ExtensionRegistry.h"
#include "registry/ImplementationRegistry.h"

#include <globjects/DebugMessage.h>
#include <globjects/logging.h>

#include <globjects/NamedString.h>

namespace
{
    bool g_globjectsIsInitialized = false;
    std::mutex g_mutex;
}

namespace glo
{

void manualErrorCheckAfter(const glbinding::AbstractFunction & function)
{
    Error error = Error::get();

    if (!error)
        return;

    if (!Registry::current().isInitialized())
    {
        debug() << "Error during initialization: " << error.name();
        return;
    }

    if (!DebugMessage::isFallbackImplementation())
        return;

    std::stringstream stream;
    stream << function.name() << " generated " << error.name();

    DebugMessage::insertMessage(gl::GL_DEBUG_SOURCE_API_ARB, gl::GL_DEBUG_TYPE_ERROR_ARB, static_cast<unsigned int>(error.code()), gl::GL_DEBUG_SEVERITY_HIGH_ARB, stream.str());
}

void init()
{
    g_mutex.lock();
    if (g_globjectsIsInitialized)
    {
        glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After, { "glGetError" });

        glbinding::setAfterCallback([](const glbinding::FunctionCall & functionCall) {
            manualErrorCheckAfter(functionCall.function);
        });

        g_globjectsIsInitialized = true;
    }
    g_mutex.unlock();

    registerCurrentContext();
}

void init(glbinding::ContextHandle sharedContextId)
{
    g_mutex.lock();
    if (g_globjectsIsInitialized)
    {
        glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After, { "glGetError" });

        glbinding::setAfterCallback([](const glbinding::FunctionCall & functionCall) {
            manualErrorCheckAfter(functionCall.function);
        });

        g_globjectsIsInitialized = true;
    }
    g_mutex.unlock();

    registerCurrentContext(sharedContextId);
}

void registerCurrentContext()
{
    glbinding::ContextHandle contextId = glbinding::getCurrentContext();

    glbinding::Binding::useContext(contextId);
    Registry::registerContext(contextId);
}

void registerCurrentContext(glbinding::ContextHandle sharedContextId)
{
    glbinding::ContextHandle contextId = glbinding::getCurrentContext();

    glbinding::Binding::useContext(contextId);
    Registry::registerContext(contextId, sharedContextId);
}

void setContext(glbinding::ContextHandle contextId)
{
    glbinding::Binding::useContext(contextId);
    Registry::setCurrentContext(contextId);
}

void setCurrentContext()
{
    glbinding::ContextHandle contextId = glbinding::getCurrentContext();
    setContext(contextId);
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

glbinding::Version version()
{
    return glbinding::Version(majorVersion(), minorVersion());
}

bool isCoreProfile()
{
    if (version()<glbinding::Version(3,2))
    {
        return false;
    }

    return (getInteger(gl::GL_CONTEXT_PROFILE_MASK) & static_cast<unsigned>(gl::GL_CONTEXT_CORE_PROFILE_BIT)) > 0;
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

bool hasExtension(gl::GLextension extension)
{
    return ExtensionRegistry::current().hasExtension(extension);
}

bool hasExtension(const std::string & extensionName)
{
    return ExtensionRegistry::current().hasExtension(extensionName);
}

bool isInCoreProfile(gl::GLextension extension, const glbinding::Version & version)
{
    return ExtensionRegistry::current().isInCoreProfile(extension, version);
}

bool isInCoreProfile(gl::GLextension extension)
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

void initializeStrategy(AbstractUniform::BindlessImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(Buffer::BindlessImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(Framebuffer::BindlessImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(DebugMessage::Implementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(Program::BinaryImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(Shader::IncludeImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(Object::NameImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(VertexArray::AttributeImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

} // namespace glo
