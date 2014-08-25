#include <globjects/DebugMessage.h>

#include <sstream>
#include <cassert>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>

#include "registry/Registry.h"
#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractDebugImplementation.h"

namespace glo {

AbstractDebugImplementation & implementation()
{
    return ImplementationRegistry::current().debugImplementation();
}

DebugMessage::DebugMessage(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, const std::string & message)
: m_source(source)
, m_type(type)
, m_id(id)
, m_severity(severity)
, m_message(message)
{
}

gl::GLenum DebugMessage::source() const
{
    return m_source;
}

gl::GLenum DebugMessage::type() const
{
    return m_type;
}

gl::GLuint DebugMessage::id() const
{
    return m_id;
}

gl::GLenum DebugMessage::severity() const
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
        case gl::GL_DEBUG_SEVERITY_HIGH_ARB:
            return "high";
        case gl::GL_DEBUG_SEVERITY_MEDIUM_ARB:
            return "medium";
        case gl::GL_DEBUG_SEVERITY_LOW_ARB:
            return "low";
        default:
            return "unknown";
    }
}

std::string DebugMessage::sourceString() const
{
    switch (m_source)
    {
        case gl::GL_DEBUG_SOURCE_API_ARB:
            return "API";
        case gl::GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
            return "Window System";
        case gl::GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
            return "Shader Compiler";
        case gl::GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
            return "Third Party";
        case gl::GL_DEBUG_SOURCE_APPLICATION_ARB:
            return "Application";
        case gl::GL_DEBUG_SOURCE_OTHER_ARB:
            return "Other";
        default:
            return "Unknown";
    }
}

std::string DebugMessage::typeString() const
{
    switch (m_type)
    {
        case gl::GL_DEBUG_TYPE_OTHER_ARB:
            return "other";
        case gl::GL_DEBUG_TYPE_ERROR_ARB:
            return "error";
        case gl::GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
            return "deprecated behavior";
        case gl::GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
            return "undefined behavior";
        case gl::GL_DEBUG_TYPE_PORTABILITY_ARB:
            return "portability";
        case gl::GL_DEBUG_TYPE_PERFORMANCE_ARB:
            return "performance";
        default:
            return "unknown";
    }
}

bool DebugMessage::isFallbackImplementation()
{
    return implementation().isFallback();
}

void DebugMessage::enable(bool synchronous)
{
    implementation().enable();

    setSynchronous(synchronous);
}

void DebugMessage::disable()
{
    implementation().disable();
}

void DebugMessage::setCallback(Callback callback)
{
    implementation().setCallback(callback);
}

void DebugMessage::setSynchronous(bool synchronous)
{
    implementation().setSynchronous(synchronous);
}

void DebugMessage::insertMessage(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, gl::GLsizei length, const char * message)
{
    assert(message != nullptr);

    insertMessage(DebugMessage(source, type, id, severity, std::string(message, length)));
}

void DebugMessage::insertMessage(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, const std::string & message)
{
    insertMessage(DebugMessage(source, type, id, severity, message));
}

void DebugMessage::insertMessage(const DebugMessage & message)
{
    implementation().insertMessage(message);
}

void DebugMessage::enableMessage(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLuint id)
{
    enableMessages(source, type, severity, 1, &id);
}

void DebugMessage::enableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids)
{
    controlMessages(source, type, severity, count, ids, gl::GL_TRUE);
}

void DebugMessage::enableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, const std::vector<gl::GLuint> & ids)
{
    enableMessages(source, type, severity, static_cast<int>(ids.size()), ids.data());
}

void DebugMessage::disableMessage(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLuint id)
{
    disableMessages(source, type, severity, 1, &id);
}

void DebugMessage::disableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids)
{
    controlMessages(source, type, severity, count, ids, gl::GL_FALSE);
}

void DebugMessage::disableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, const std::vector<gl::GLuint> & ids)
{
    disableMessages(source, type, severity, static_cast<int>(ids.size()), ids.data());
}

void DebugMessage::controlMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids, gl::GLboolean enabled)
{
    assert(ids != nullptr || count == 0);

    implementation().controlMessages(source, type, severity, count, ids, enabled);
}

} // namespace glo
