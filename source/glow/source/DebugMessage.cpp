#include <glow/DebugMessage.h>

#include <sstream>

namespace glow {

DebugMessage::DebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, const std::string & message)
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

} // namespace glow
