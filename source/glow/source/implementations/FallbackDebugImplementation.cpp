#include "FallbackDebugImplementation.h"


namespace glow {

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
        m_messageCallback(message);
}

void FallbackDebugImplementation::controlMessages(GLenum , GLenum , GLenum , GLsizei , const GLuint * , GLboolean )
{
    // ignore
}

} // namespace glow
