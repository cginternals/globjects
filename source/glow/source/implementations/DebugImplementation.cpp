#include "DebugImplementation.h"

#include <glow/glow.h>

#ifdef WIN32
#include <Windows.h>
#else
#define APIENTRY
#endif

namespace glow {

void APIENTRY DebugImplementation::debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char * message, const void * param)
{
    if (!param)
        return;

    const DebugMessageCallback & messageCallback = *reinterpret_cast<const DebugMessageCallback*>(param);

    messageCallback(glow::DebugMessage(source, type, id, severity, std::string(message, length)));
}

void DebugImplementation::registerCallback()
{
    if (m_messageCallback.isRegistered())
        return;

    glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(debugMessageCallback), reinterpret_cast<const void*>(&m_messageCallback));

    m_messageCallback.setRegistered(true);
}

void DebugImplementation::enable()
{
    glow::enable(GL_DEBUG_OUTPUT);

    registerCallback();
}

void DebugImplementation::disable()
{
    glow::disable(GL_DEBUG_OUTPUT);
}

void DebugImplementation::setSynchronous(bool synchronous)
{
    setEnabled(GL_DEBUG_OUTPUT_SYNCHRONOUS, synchronous);
}

void DebugImplementation::insertMessage(const DebugMessage & message)
{
    if (message.isManualErrorMessage())
        return;

    glDebugMessageInsert(message.source(), message.type(), message.id(), message.severity(), static_cast<GLsizei>(message.message().length()), message.message().c_str());
}

void DebugImplementation::controlMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)
{
    glDebugMessageControl(source, type, severity, count, ids, enabled);
}

} // namespace glow
