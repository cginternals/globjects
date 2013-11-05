#include <algorithm>
#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
#include <stdexcept>
#endif

#include <glow/logging.h>
#include <glow/Error.h>

#include <glow/DebugMessage.h>
#include <glow/DebugMessageOutput.h>

#ifdef WIN32

#elif __APPLE__
//#include <GL/glxew.h>
#else
#include <GL/glxew.h>
#endif

namespace 
{
    long long getContextHandle()
    {
        long long handle = 0;
#ifdef WIN32
        const int contextID = reinterpret_cast<int>(wglGetCurrentContext());
        handle = static_cast<long long>(contextID);
#elif __APPLE__
        //const GLXContext context = glXGetCurrentContext();
        //handle = reinterpret_cast<long long>(context);
#else
        const GLXContext context = glXGetCurrentContext();
        handle = reinterpret_cast<long long>(context);
#endif
        return handle;
    }
}

namespace glow
{

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
