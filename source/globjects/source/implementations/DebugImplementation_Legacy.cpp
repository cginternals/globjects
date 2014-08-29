
#include "DebugImplementation_Legacy.h"


using namespace gl;

namespace glo 
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
}

void DebugImplementation_Legacy::disable()
{
    m_enabled = false;
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

} // namespace glo
