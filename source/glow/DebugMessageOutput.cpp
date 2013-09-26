
#include <cassert>
#include <sstream>
#include <algorithm>
#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
#include <stdexcept>
#endif

#include <glow/logging.h>
#include <glow/DebugMessageOutput.h>
#include <glow/Error.h>

#ifdef WIN32
#include <GL/wglew.h>
#elif __APPLE__
#else
#include <GL/glxew.h>
#endif


namespace {
    long long getContextHandle()
    {
        long long handle = 0;
#ifdef WIN32
        const int contextID = reinterpret_cast<int>(wglGetCurrentContext());
        handle = static_cast<long long>(contextID);
#elif __APPLE__

#else
        const GLXContext context = glXGetCurrentContext();
        handle = reinterpret_cast<long long>(context);
#endif
        return handle;
    }
}

namespace glow
{

DebugMessage::DebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, const std::string& message)
: source(source)
, type(type)
, id(id)
, severity(severity)
, message(message)
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

    stream
            << typeString(type)
            << ": " << std::hex << "0x" << id << std::dec
            << ", " << severityString(severity) << " severity"
            << " (" << sourceString(source) << ")"
            << std::endl
            << message;

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



bool DebugMessageOutput::s_errorChecking = false;
int DebugMessageOutput::s_nextId = 0;
std::unordered_map<long long, int> DebugMessageOutput::s_ids;
std::unordered_map<int, std::vector<DebugMessageOutput::Callback>> DebugMessageOutput::s_callbacks;

unsigned DebugMessageOutput::getId()
{
    long long handle = getContextHandle();

    if (s_ids.find(handle) == s_ids.end())
    {
        s_ids[handle] = s_nextId++;
    }

    return s_ids[handle];
}

void DebugMessageOutput::enable()
{
    if (!GLEW_ARB_debug_output)
    {
        s_errorChecking = true;
        return;
    }

    int id = getId();

    glDebugMessageCallback(&DebugMessageOutput::handleMessage, reinterpret_cast<void*>(id));
    CheckGLError();

    glEnable(GL_DEBUG_OUTPUT);

    setSynchronous(true);
}

void DebugMessageOutput::disable()
{
    if (!GLEW_ARB_debug_output)
    {
        s_errorChecking = false;
        return;
    }

    glDisable(GL_DEBUG_OUTPUT);
}

void DebugMessageOutput::setSynchronous(bool synchronous)
{
    if (!GLEW_ARB_debug_output)
        return;

    if (synchronous)
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    else
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}

void DebugMessageOutput::addCallback(Callback callback)
{
    s_callbacks[getId()].push_back(callback);
}

void DebugMessageOutput::insertMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message)
{
    if (!GLEW_ARB_debug_output)
        return;

    glDebugMessageInsert(source, type, id, severity, length, message);
    CheckGLError();
}

void DebugMessageOutput::insertMessage(GLenum source, GLenum type, GLuint id, GLenum severity, const std::string& message)
{
    insertMessage(source, type, id, severity, message.length(), message.c_str());
}

void DebugMessageOutput::insertMessage(const DebugMessage& message)
{
    insertMessage(message.source, message.type, message.id, message.severity, message.message);
}

void DebugMessageOutput::enableMessage(GLenum source, GLenum type, GLenum severity, GLuint id)
{
    enableMessages(source, type, severity, 1, &id);
}

void DebugMessageOutput::enableMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids)
{
    glDebugMessageControl(source, type, severity, count, ids, GL_TRUE);
    CheckGLError();
}

void DebugMessageOutput::enableMessages(GLenum source, GLenum type, GLenum severity, const std::vector<GLuint>& ids)
{
    enableMessages(source, type, severity, ids.size(), ids.data());
}

void DebugMessageOutput::disableMessage(GLenum source, GLenum type, GLenum severity, GLuint id)
{
    disableMessages(source, type, severity, 1, &id);
}

void DebugMessageOutput::disableMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids)
{
    glDebugMessageControl(source, type, severity, count, ids, GL_FALSE);
}

void DebugMessageOutput::disableMessages(GLenum source, GLenum type, GLenum severity, const std::vector<GLuint>& ids)
{
    disableMessages(source, type, severity, ids.size(), ids.data());
}

void APIENTRY DebugMessageOutput::handleMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char * message, void * param)
{
    handleMessage(DebugMessage(source, type, id, severity, message), *reinterpret_cast<int*>(&param));
}

void DebugMessageOutput::handleMessage(const DebugMessage& message, int id)
{
    if (s_callbacks[id].empty())
    {
        if (message.type == GL_DEBUG_TYPE_ERROR_ARB)
        {
#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
            if (message.type == GL_DEBUG_TYPE_ERROR_ARB)
                throw std::runtime_error(message.toString());
#else
            fatal() << message.toString();
#endif
        }
        else
        {
            debug() << message.toString();
        }
    }
    else
    {
        for (Callback callback: s_callbacks[id])
        {
            callback(message);
        }
    }
}

void DebugMessageOutput::checkError(const char* file, int line)
{
    if (!s_errorChecking)
        return;

    Error error = Error::get();

    if (!error)
        return;

    handleMessage(DebugMessage(GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_ERROR_ARB, error.code(), GL_DEBUG_SEVERITY_HIGH_ARB, error.name()), getId());
}

} // namespace glow
