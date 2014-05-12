#include <glow/DebugMessage.h>

#include <sstream>

#include <glbinding/constants.h>

namespace glow {

DebugMessage::DebugMessage(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, const std::string & message)
: m_source(source)
, m_type(type)
, m_id(id)
, m_severity(severity)
, m_message(message)
{
}

bool DebugMessage::isManualErrorMessage() const
{
    return false;
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
        case gl::DEBUG_SEVERITY_HIGH_ARB:
            return "high";
        case gl::DEBUG_SEVERITY_MEDIUM_ARB:
            return "medium";
        case gl::DEBUG_SEVERITY_LOW_ARB:
            return "low";
        default:
            return "unknown";
    }
}

std::string DebugMessage::sourceString() const
{
    switch (m_source)
    {
        case gl::DEBUG_SOURCE_API_ARB:
            return "API";
        case gl::DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
            return "Window System";
        case gl::DEBUG_SOURCE_SHADER_COMPILER_ARB:
            return "Shader Compiler";
        case gl::DEBUG_SOURCE_THIRD_PARTY_ARB:
            return "Third Party";
        case gl::DEBUG_SOURCE_APPLICATION_ARB:
            return "Application";
        case gl::DEBUG_SOURCE_OTHER_ARB:
            return "Other";
        default:
            return "Unknown";
    }
}

std::string DebugMessage::typeString() const
{
    switch (m_type)
    {
        case gl::DEBUG_TYPE_OTHER_ARB:
            return "other";
        case gl::DEBUG_TYPE_ERROR_ARB:
            return "error";
        case gl::DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
            return "deprecated behavior";
        case gl::DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
            return "undefined behavior";
        case gl::DEBUG_TYPE_PORTABILITY_ARB:
            return "portability";
        case gl::DEBUG_TYPE_PERFORMANCE_ARB:
            return "performance";
        default:
            return "unknown";
    }
}

} // namespace glow
