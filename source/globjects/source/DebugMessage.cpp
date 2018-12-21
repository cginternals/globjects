
#include <globjects/DebugMessage.h>

#include <sstream>
#include <cassert>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>

#include "registry/Registry.h"
#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractDebugImplementation.h"


using namespace gl;


namespace
{


globjects::AbstractDebugImplementation & implementation()
{
    return globjects::ImplementationRegistry::current().debugImplementation();
}


} // namespace


namespace globjects 
{


void DebugMessage::hintImplementation(const Implementation impl)
{
    ImplementationRegistry::current().initialize(impl);
}

DebugMessage::DebugMessage(const GLenum source, const GLenum type, const GLuint id, const GLenum severity, const std::string & message)
: m_source(source)
, m_type(type)
, m_id(id)
, m_severity(severity)
, m_message(message)
{
}

GLenum DebugMessage::source() const
{
    return m_source;
}

GLenum DebugMessage::type() const
{
    return m_type;
}

GLuint DebugMessage::id() const
{
    return m_id;
}

GLenum DebugMessage::severity() const
{
    return m_severity;
}

const std::string & DebugMessage::message() const
{
    return m_message;
}

std::string DebugMessage::toString() const
{
    std::stringstream stream;

    stream
        << typeString()
        << ": " << std::hex << "0x" << m_id << std::dec
        << ", " << severityString() << " severity"
        << " (" << sourceString() << ")"
        << std::endl
        << "\t" << m_message;

    return stream.str();
}

std::string DebugMessage::severityString() const
{
    switch (m_severity)
    {
        case GL_DEBUG_SEVERITY_HIGH_ARB:
            return "high";
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:
            return "medium";
        case GL_DEBUG_SEVERITY_LOW_ARB:
            return "low";
        default:
            return "unknown";
    }
}

std::string DebugMessage::sourceString() const
{
    switch (m_source)
    {
        case GL_DEBUG_SOURCE_API_ARB:
            return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
            return "Window System";
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
            return "Shader Compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
            return "Third Party";
        case GL_DEBUG_SOURCE_APPLICATION_ARB:
            return "Application";
        case GL_DEBUG_SOURCE_OTHER_ARB:
            return "Other";
        default:
            return "Unknown";
    }
}

std::string DebugMessage::typeString() const
{
    switch (m_type)
    {
        case GL_DEBUG_TYPE_OTHER_ARB:
            return "other";
        case GL_DEBUG_TYPE_ERROR_ARB:
            return "error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
            return "deprecated behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
            return "undefined behavior";
        case GL_DEBUG_TYPE_PORTABILITY_ARB:
            return "portability";
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:
            return "performance";
        default:
            return "unknown";
    }
}

bool DebugMessage::isFallbackImplementation()
{
    return implementation().isFallback();
}

void DebugMessage::enable(const bool synchronous)
{
    implementation().enable();

    setSynchronous(synchronous);
}

void DebugMessage::disable()
{
    implementation().disable();
}

bool DebugMessage::isEnabled()
{
    return implementation().isEnabled();
}

void DebugMessage::setCallback(Callback callback)
{
    implementation().setCallback(callback);
}

void DebugMessage::setSynchronous(const bool synchronous)
{
    implementation().setSynchronous(synchronous);
}

bool DebugMessage::isSynchronous()
{
    return implementation().isSynchronous();
}

void DebugMessage::insertMessage(const GLenum source, const GLenum type, const GLuint id, const GLenum severity, const GLsizei length, const char * message)
{
    assert(message != nullptr);

    insertMessage(DebugMessage(source, type, id, severity, std::string(message, length)));
}

void DebugMessage::insertMessage(const GLenum source, const GLenum type, const GLuint id, const GLenum severity, const std::string & message)
{
    insertMessage(DebugMessage(source, type, id, severity, message));
}

void DebugMessage::insertMessage(const DebugMessage & message)
{
    implementation().insertMessage(message);
}

void DebugMessage::enableMessage(const GLenum source, const GLenum type, const GLenum severity, GLuint id)
{
    enableMessages(source, type, severity, 1, &id);
}

void DebugMessage::enableMessages(const GLenum source, const GLenum type, const GLenum severity, const GLsizei count, const GLuint * ids)
{
    controlMessages(source, type, severity, count, ids, GL_TRUE);
}

void DebugMessage::enableMessages(const GLenum source, const GLenum type, const GLenum severity, const std::vector<GLuint> & ids)
{
    enableMessages(source, type, severity, static_cast<int>(ids.size()), ids.data());
}

void DebugMessage::disableMessage(const GLenum source, const GLenum type, const GLenum severity, const GLuint id)
{
    disableMessages(source, type, severity, 1, &id);
}

void DebugMessage::disableMessages(const GLenum source, const GLenum type, const GLenum severity, const GLsizei count, const GLuint * ids)
{
    controlMessages(source, type, severity, count, ids, GL_FALSE);
}

void DebugMessage::disableMessages(const GLenum source, const GLenum type, const GLenum severity, const std::vector<GLuint> & ids)
{
    disableMessages(source, type, severity, static_cast<int>(ids.size()), ids.data());
}

void DebugMessage::controlMessages(const GLenum source, const GLenum type, const GLenum severity, const GLsizei count, const GLuint * ids, const GLboolean enabled)
{
    assert(ids != nullptr || count == 0);

    implementation().controlMessages(source, type, severity, count, ids, enabled);
}


} // namespace globjects
