#include "DebugImplementation.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <glow/DebugMessage.h>

#include <glow/glow.h>

namespace glow {

void GL_APIENTRY DebugImplementation::debugMessageCallback(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, gl::GLsizei length, const char * message, const void * param)
{
    if (!param)
        return;

    const DebugImplementation * debugImplementation = reinterpret_cast<const DebugImplementation*>(param);

    debugImplementation->handleMessage(DebugMessage(source, type, id, severity, std::string(message, length)));
}

DebugImplementation::DebugImplementation()
: m_isRegistered(false)
{
}

void DebugImplementation::registerCallback()
{
    if (m_isRegistered)
        return;

    gl::glDebugMessageCallback(reinterpret_cast<gl::GLDEBUGPROC>(debugMessageCallback), reinterpret_cast<void*>(this));

    m_isRegistered = true;
}

void DebugImplementation::enable()
{
    glow::enable(gl::GL_DEBUG_OUTPUT);

    registerCallback();
}

void DebugImplementation::disable()
{
    glow::disable(gl::GL_DEBUG_OUTPUT);
}

void DebugImplementation::setSynchronous(bool synchronous)
{
    setEnabled(gl::GL_DEBUG_OUTPUT_SYNCHRONOUS, synchronous);
}

void DebugImplementation::insertMessage(const DebugMessage & message)
{
    gl::glDebugMessageInsert(message.source(), message.type(), message.id(), message.severity(), static_cast<gl::GLsizei>(message.message().length()), message.message().c_str());
}

void DebugImplementation::controlMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids, gl::GLboolean enabled)
{
    gl::glDebugMessageControl(source, type, severity, count, ids, enabled);
}

} // namespace glow
