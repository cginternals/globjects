#include "DebugImplementation.h"

#include <glbinding/constants.h>
#include <glbinding/functions.h>

#include <glow/glow.h>

#ifdef WIN32
#include <Windows.h>
#else
#define APIENTRY
#endif

namespace glow {

void APIENTRY DebugImplementation::debugMessageCallback(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, gl::GLsizei length, const char * message, const void * param)
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

    gl::DebugMessageCallback(reinterpret_cast<gl::GLDEBUGPROC>(debugMessageCallback), reinterpret_cast<const void*>(&m_messageCallback));

    m_messageCallback.setRegistered(true);
}

void DebugImplementation::enable()
{
    glow::enable(gl::DEBUG_OUTPUT);

    registerCallback();
}

void DebugImplementation::disable()
{
    glow::disable(gl::DEBUG_OUTPUT);
}

void DebugImplementation::setSynchronous(bool synchronous)
{
    setEnabled(gl::DEBUG_OUTPUT_SYNCHRONOUS, synchronous);
}

void DebugImplementation::insertMessage(const DebugMessage & message)
{
    if (message.isManualErrorMessage())
        return;

    gl::DebugMessageInsert(message.source(), message.type(), message.id(), message.severity(), static_cast<gl::GLsizei>(message.message().length()), message.message().c_str());
}

void DebugImplementation::controlMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids, gl::GLboolean enabled)
{
    gl::DebugMessageControl(source, type, severity, count, ids, enabled);
}

} // namespace glow
