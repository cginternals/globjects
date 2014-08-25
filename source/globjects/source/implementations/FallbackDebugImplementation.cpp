#include "FallbackDebugImplementation.h"

namespace glo {

FallbackDebugImplementation::FallbackDebugImplementation()
: m_enabled(false)
{
}

bool FallbackDebugImplementation::isFallback()
{
    return true;
}

void FallbackDebugImplementation::enable()
{
    m_enabled = true;
}

void FallbackDebugImplementation::disable()
{
    m_enabled = false;
}

void FallbackDebugImplementation::setSynchronous(bool)
{
    // ignore
}

void FallbackDebugImplementation::insertMessage(const DebugMessage & message)
{
    if (m_enabled)
    {
        handleMessage(message);
    }
}

void FallbackDebugImplementation::controlMessages(gl::GLenum , gl::GLenum , gl::GLenum , gl::GLsizei , const gl::GLuint * , gl::GLboolean )
{
    // ignore
    // TODO: emulate OpenGL's filtering functionality
}

} // namespace glo
