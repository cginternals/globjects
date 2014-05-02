#include <algorithm>
#include <cassert>
#include <unordered_map>

#include <glow/logging.h>
#include <glow/Error.h>
#include <glow/DebugMessage.h>
#include <glow/glow.h>
#include <glow/Extension.h>

#include "registry/contextid.h"
#include "DebugMessageCallback.h"

#ifdef WIN32
#include <Windows.h>
#else
#define APIENTRY
#endif

#include <glow/debugmessageoutput.h>

namespace {

bool manualErrorCheckFallbackEnabled = false;
std::unordered_map<long long, glow::DebugMessageCallback> callbackStates;

}

namespace glow
{

DebugMessageCallback * currentDebugMessageCallback()
{
    long long id = getContextId();

    return &callbackStates[id];
}

void APIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char * message, const void * param)
{
    if (!param)
        return;

    const DebugMessageCallback & messageCallback = *reinterpret_cast<const DebugMessageCallback*>(param);
    messageCallback(glow::DebugMessage(source, type, id, severity, std::string(message, length)));
}

void registerDebugMessageCallback(DebugMessageCallback * messageCallback)
{
    if (glow::hasExtension(GLOW_KHR_debug))
    {
        if (!messageCallback->isRegistered())
        {
            glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(debugMessageCallback), reinterpret_cast<const void*>(messageCallback));
            messageCallback->setRegistered(true);
        }
    }
}

namespace debugmessageoutput
{

void setCallback(Callback callback)
{
    DebugMessageCallback* messageCallback = currentDebugMessageCallback();
    messageCallback->clearCallbacks();
    messageCallback->addCallback(callback);

    registerDebugMessageCallback(messageCallback);
}

void addCallback(Callback callback)
{
    DebugMessageCallback* messageCallback = currentDebugMessageCallback();
    messageCallback->addCallback(callback);

    registerDebugMessageCallback(messageCallback);
}


void enable(bool synchronous, bool registerDefaultCallback)
{
    if (!glow::hasExtension(GLOW_KHR_debug))
    {
        manualErrorCheckFallbackEnabled = true;

        return;
    }

    glow::enable(GL_DEBUG_OUTPUT);

    setSynchronous(synchronous);

    if (registerDefaultCallback)
    {
        registerDebugMessageCallback(currentDebugMessageCallback());
    }
}

void disable()
{
    if (!glow::hasExtension(GLOW_KHR_debug))
    {
        manualErrorCheckFallbackEnabled = false;

        return;
    }

    glow::disable(GL_DEBUG_OUTPUT);
}

void setSynchronous(bool synchronous)
{
    if (!glow::hasExtension(GLOW_KHR_debug))
        return;

    setEnabled(GL_DEBUG_OUTPUT_SYNCHRONOUS, synchronous);
}

void insertMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message)
{
    assert(message != nullptr);

    if (!glow::hasExtension(GLOW_KHR_debug))
        return;

    glDebugMessageInsert(source, type, id, severity, length, message);
    CheckGLError();
}

void insertMessage(GLenum source, GLenum type, GLuint id, GLenum severity, const std::string& message)
{
    insertMessage(source, type, id, severity, static_cast<int>(message.length()), message.c_str());
}

void insertMessage(const DebugMessage& message)
{
    insertMessage(message.source, message.type, message.id, message.severity, message.message);
}

void enableMessage(GLenum source, GLenum type, GLenum severity, GLuint id)
{
    enableMessages(source, type, severity, 1, &id);
}

void enableMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids)
{
    controlMessages(source, type, severity, count, ids, GL_TRUE);
}

void enableMessages(GLenum source, GLenum type, GLenum severity, const std::vector<GLuint>& ids)
{
    enableMessages(source, type, severity, static_cast<int>(ids.size()), ids.data());
}

void disableMessage(GLenum source, GLenum type, GLenum severity, GLuint id)
{
    disableMessages(source, type, severity, 1, &id);
}

void disableMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids)
{
    controlMessages(source, type, severity, count, ids, GL_FALSE);
}

void disableMessages(GLenum source, GLenum type, GLenum severity, const std::vector<GLuint>& ids)
{
    disableMessages(source, type, severity, static_cast<int>(ids.size()), ids.data());
}

void controlMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)
{
    if (!glow::hasExtension(GLOW_KHR_debug))
        return;

    assert(ids != nullptr || count == 0);

    glDebugMessageControl(source, type, severity, count, ids, enabled);
    CheckGLError();
}


void manualErrorCheck(const char* file, int line)
{
    if (!manualErrorCheckFallbackEnabled)
        return;

    Error error = Error::get();

    if (!error)
        return;

    DebugMessageCallback & messageCallback = *currentDebugMessageCallback();
    messageCallback(DebugMessage(GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_ERROR_ARB, error.code(), GL_DEBUG_SEVERITY_HIGH_ARB, error.name(), file, line));
}

} // namespace debugmessageoutput
} // namespace glow
