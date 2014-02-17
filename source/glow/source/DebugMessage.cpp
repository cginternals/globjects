#include <glow/DebugMessage.h>

#include <sstream>

namespace glow {

DebugMessage::DebugMessage(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    const std::string& message
)
: source(source)
, type(type)
, id(id)
, severity(severity)
, message(message)
, file(nullptr)
, line(0)
{
}

DebugMessage::DebugMessage(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    const std::string& message,
    const char* file, int line
)
: source(source)
, type(type)
, id(id)
, severity(severity)
, message(message)
, file(file)
, line(line)
{
}

const char* DebugMessage::severityString() const
{
    return severityString(severity);
}

const char* DebugMessage::sourceString() const
{
    return sourceString(source);
}

const char* DebugMessage::typeString() const
{
    return typeString(type);
}

std::string DebugMessage::toString() const
{
    std::stringstream stream;

    stream << typeString(type);
    if (file) {
        stream << " [" << file << ":" << line << "]";
    }
    stream
    << ": " << std::hex << "0x" << id << std::dec
    << ", " << severityString(severity) << " severity"
    << " (" << sourceString(source) << ")"
    << std::endl
    << "\t" << message;

    return stream.str();
}

const char* DebugMessage::severityString(GLenum severity)
{
    switch (severity)
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

const char* DebugMessage::sourceString(GLenum source)
{
    switch (source)
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

const char* DebugMessage::typeString(GLenum type)
{
    switch (type)
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
