
#include "DebugImplementation_Legacy.h"

#include <glbinding/callbacks.h>


using namespace gl;

namespace globjects 
{

DebugImplementation_Legacy::DebugImplementation_Legacy()
: m_enabled(false)
{
}

bool DebugImplementation_Legacy::isFallback()
{
    return true;
}

void DebugImplementation_Legacy::enable()
{
    m_enabled = true;

    glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After, { "glGetError" });
}

void DebugImplementation_Legacy::disable()
{
    m_enabled = false;

    glbinding::setCallbackMask(glbinding::CallbackMask::None);
}

void DebugImplementation_Legacy::setSynchronous(bool)
{
    // ignore
}

void DebugImplementation_Legacy::insertMessage(const DebugMessage & message)
{
    if (m_enabled)
    {
        handleMessage(message);
    }
}

void DebugImplementation_Legacy::controlMessages(GLenum , GLenum , GLenum , GLsizei , const GLuint *, GLboolean )
{
    // ignore
    // TODO: emulate OpenGL's filtering functionality
}

} // namespace globjects
