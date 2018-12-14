
#include "DebugImplementation_DebugKHR.h"

#include <glbinding/glbinding.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/DebugMessage.h>

#include <globjects/globjects.h>


using namespace gl;


namespace globjects
{


void GL_APIENTRY DebugImplementation_DebugKHR::debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char * message, const void * param)
{
    if (!param)
    {
        return;
    }

    const DebugImplementation_DebugKHR * debugImplementation = reinterpret_cast<const DebugImplementation_DebugKHR*>(param);

    debugImplementation->handleMessage(DebugMessage(source, type, id, severity, std::string(message, length)));
}

DebugImplementation_DebugKHR::DebugImplementation_DebugKHR()
: m_isRegistered(false)
{
    registerCallback();
}

void DebugImplementation_DebugKHR::registerCallback()
{
    if (m_isRegistered)
        return;

    glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(debugMessageCallback), reinterpret_cast<void*>(this));

    m_isRegistered = true;
}

void DebugImplementation_DebugKHR::enable()
{
    globjects::enable(GL_DEBUG_OUTPUT);
}

void DebugImplementation_DebugKHR::disable()
{
    globjects::disable(GL_DEBUG_OUTPUT);
}

bool DebugImplementation_DebugKHR::isEnabled() const
{
    return globjects::isEnabled(GL_DEBUG_OUTPUT);
}

void DebugImplementation_DebugKHR::setSynchronous(bool synchronous)
{
    setEnabled(GL_DEBUG_OUTPUT_SYNCHRONOUS, synchronous);
}

bool DebugImplementation_DebugKHR::isSynchronous() const
{
    return globjects::isEnabled(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}

void DebugImplementation_DebugKHR::insertMessage(const DebugMessage & message)
{
    glDebugMessageInsert(message.source(), message.type(), message.id(), message.severity(), static_cast<GLsizei>(message.message().length()), message.message().c_str());
}

void DebugImplementation_DebugKHR::controlMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)
{
    glDebugMessageControl(source, type, severity, count, ids, enabled);
}


} // namespace globjects
