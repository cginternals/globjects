#include <glow/Error.h>

#include <glow/DebugMessage.h>
#include "debugmessageoutput_private.h"

namespace glow
{


Error::Error(GLenum errorCode)
: m_errorCode(errorCode)
{
}

Error::Error()
: Error(GL_NO_ERROR)
{
}

GLenum Error::code() const
{
	return m_errorCode;
}

std::string Error::name() const
{
    switch(m_errorCode)
    {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "Unknown GLenum.";
    }
}

Error Error::get()
{
	return Error(glGetError());
}

void Error::clear()
{
    while (Error::get());
}

bool Error::isError() const
{
	return m_errorCode != GL_NO_ERROR;
}

Error::operator bool() const
{
    return isError();
}

void Error::check(const char * file, int line)
{
    Error error = Error::get();

    if (!error)
        return;

    debugmessageoutput::signalError(error, file, line);
}

} // namespace glow
