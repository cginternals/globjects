#include <glow/Error.h>

#include <glbinding/constants.h>

#include <glow/DebugMessage.h>
#include "debugmessageoutput_private.h"

namespace glow
{


Error::Error(GLenum errorCode)
: m_errorCode(errorCode)
{
}

Error::Error()
: Error(gl::NO_ERROR)
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
        case gl::NO_ERROR:
            return "gl::NO_ERROR";
        case gl::INVALID_ENUM:
            return "gl::INVALID_ENUM";
        case gl::INVALID_VALUE:
            return "gl::INVALID_VALUE";
        case gl::INVALID_OPERATION:
            return "gl::INVALID_OPERATION";
        case gl::INVALID_FRAMEBUFFER_OPERATION:
            return "gl::INVALID_FRAMEBUFFER_OPERATION";
        case gl::OUT_OF_MEMORY:
            return "gl::OUT_OF_MEMORY";
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
	return m_errorCode != gl::NO_ERROR;
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
