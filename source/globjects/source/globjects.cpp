#include <globjects/globjects.h>

#include <unordered_map>
#include <mutex>

#include <glbinding/gl/gl.h>
#include <glbinding/gl/functions.h>
#include <glbinding/AbstractFunction.h>
#include <glbinding/Binding.h>
#include <glbinding/callbacks.h>
#include <glbinding/ContextInfo.h>

#include <globjects/Error.h>
#include <globjects/logging.h>

#include "registry/Registry.h"
#include "registry/ObjectRegistry.h"
#include "registry/ExtensionRegistry.h"
#include "registry/ImplementationRegistry.h"

#include <globjects/DebugMessage.h>
#include <globjects/logging.h>

#include <globjects/NamedString.h>


using namespace gl;

namespace
{
    bool g_globjectsIsInitialized = false;
    std::mutex g_mutex;
}

namespace globjects
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

    DebugMessage::insertMessage(GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_ERROR_ARB, static_cast<unsigned int>(error.code()), GL_DEBUG_SEVERITY_HIGH_ARB, stream.str());
}

void init()
{
    g_mutex.lock();
    if (!g_globjectsIsInitialized)
    {
        glbinding::setAfterCallback([](const glbinding::FunctionCall & functionCall) {
            manualErrorCheckAfter(*functionCall.function); });

        g_globjectsIsInitialized = true;
    }

    g_mutex.unlock();

    registerCurrentContext();
}

void init(const glbinding::ContextHandle sharedContextId)
{
    g_mutex.lock();
    if (g_globjectsIsInitialized)
    {
        glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After, { "glGetError" });

        glbinding::setAfterCallback([](const glbinding::FunctionCall & functionCall) {
            manualErrorCheckAfter(*functionCall.function); });

        g_globjectsIsInitialized = true;
    }
    g_mutex.unlock();

    registerCurrentContext(sharedContextId);
}

void detachAllObjects()
{
    for (Object * object : Registry::current().objects().objects())
        object->detach();
}

void registerCurrentContext()
{
    glbinding::ContextHandle contextId = glbinding::getCurrentContext();

    glbinding::Binding::useContext(contextId);
    Registry::registerContext(contextId);
}

void registerCurrentContext(const glbinding::ContextHandle sharedContextId)
{
    glbinding::ContextHandle contextId = glbinding::getCurrentContext();

    glbinding::Binding::useContext(contextId);
    Registry::registerContext(contextId, sharedContextId);
}

void setContext(const glbinding::ContextHandle contextId)
{
    glbinding::Binding::useContext(contextId);
    Registry::setCurrentContext(contextId);
}

void setCurrentContext()
{
    glbinding::ContextHandle contextId = glbinding::getCurrentContext();
    setContext(contextId);
}

std::string getString(const GLenum pname)
{
    const GLubyte* result = glGetString(pname);

	return reinterpret_cast<const char*>(result);
}

std::string getString(const GLenum pname, const GLuint index)
{
    const GLubyte* result = glGetStringi(pname, index);

    return reinterpret_cast<const char*>(result);
}

GLint getInteger(const GLenum pname)
{
	GLint value;

    glGetIntegerv(pname, &value);

	return value;
}

GLenum getEnum(const GLenum pname)
{
    return static_cast<GLenum>(getInteger(pname));
}

GLfloat getFloat(const GLenum pname)
{
	GLfloat value;

    glGetFloatv(pname, &value);

	return value;
}

GLdouble getDouble(const GLenum pname)
{
	GLdouble value;

    glGetDoublev(pname, &value);

	return value;
}

GLboolean getBoolean(const GLenum pname)
{
	GLboolean value;

    glGetBooleanv(pname, &value);

	return value;
}

GLint getInteger(const GLenum pname, const GLuint index)
{
	GLint value;

    glGetIntegeri_v(pname, index, &value);

	return value;
}

GLenum getEnum(const GLenum pname, const GLuint index)
{
    return static_cast<GLenum>(getInteger(pname, index));
}

GLfloat getFloat(const GLenum pname, const GLuint index)
{
    GLfloat value;

    glGetFloati_v(pname, index, &value);

    return value;
}

GLdouble getDouble(const GLenum pname, const GLuint index)
{
    GLdouble value;

    glGetDoublei_v(pname, index, &value);

    return value;
}

GLboolean getBoolean(const GLenum pname, const GLuint index)
{
    GLboolean value;

    glGetBooleani_v(pname, index, &value);

    return value;
}

std::string vendor()
{
    return glbinding::ContextInfo::vendor();
}

std::string renderer()
{
    return glbinding::ContextInfo::renderer();
}

std::string versionString()
{
    return getString(GL_VERSION);
}

glbinding::Version version()
{
    return glbinding::ContextInfo::version();
}

bool isCoreProfile()
{
    if (version()<glbinding::Version(3,2))
    {
        return false;
    }

    return (getInteger(GL_CONTEXT_PROFILE_MASK) & static_cast<unsigned>(GL_CONTEXT_CORE_PROFILE_BIT)) > 0;
}

const std::set<GLextension> & availableExtensions()
{
    return ExtensionRegistry::current().availableExtensions();
}

const std::set<std::string> & unknownAvailableExtensions()
{
    return ExtensionRegistry::current().unknownAvailableExtensions();
}

bool hasExtension(const GLextension extension)
{
    return ExtensionRegistry::current().hasExtension(extension);
}

bool hasExtension(const std::string & extensionName)
{
    return ExtensionRegistry::current().hasExtension(extensionName);
}

bool isInCoreProfile(const GLextension extension, const glbinding::Version & version)
{
    return ExtensionRegistry::current().isInCoreProfile(extension, version);
}

bool isInCoreProfile(const GLextension extension)
{
    return ExtensionRegistry::current().isInCoreProfile(extension);
}

void enable(const GLenum capability)
{
    glEnable(capability);
}

void disable(const GLenum capability)
{
    glDisable(capability);
}

bool isEnabled(const GLenum capability)
{
    GLboolean value = glIsEnabled(capability);

    return value == GL_TRUE;
}

void setEnabled(const GLenum capability, const bool enabled)
{
    enabled ? enable(capability) : disable(capability);
}

void enable(const GLenum capability, const int index)
{
    glEnablei(capability, index);
}

void disable(const GLenum capability, const int index)
{
    glDisablei(capability, index);
}

bool isEnabled(const GLenum capability, const int index)
{
    GLboolean value = glIsEnabledi(capability, index);

    return value == GL_TRUE;
}

void setEnabled(const GLenum capability, const int index, const bool enabled)
{
    enabled ? enable(capability, index) : disable(capability, index);
}

void initializeStrategy(const AbstractUniform::BindlessImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(const Buffer::BindlessImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(const Framebuffer::BindlessImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(const DebugMessage::Implementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(const Program::BinaryImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(const Shader::IncludeImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(const Object::NameImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

void initializeStrategy(const VertexArray::AttributeImplementation impl)
{
    Registry::current().implementations().initialize(impl);
}

} // namespace globjects
