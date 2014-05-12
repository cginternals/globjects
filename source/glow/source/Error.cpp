#include <glow/Error.h>

#include <glbinding/constants.h>
#include <glbinding/functions.h>

#include <glow/DebugMessage.h>
#include "debugmessageoutput_private.h"

namespace glow
{


Error::Error(gl::GLenum errorCode)
: m_errorCode(errorCode)
{
}

Error::Error()
: Error(gl::NO_ERROR_)
{
}

gl::GLenum Error::code() const
{
	return m_errorCode;
}

std::string Error::name() const
{
    switch(m_errorCode)
    {
        case gl::NO_ERROR_:
            return "GL_NO_ERROR";
        case gl::INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case gl::INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case gl::INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case gl::INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case gl::OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "Unknown GLenum.";
    }
}

Error Error::get()
{
    return Error(gl::GetError());
}

void Error::clear()
{
    while (Error::get());
}

bool Error::isError() const
{
	return m_errorCode != gl::NO_ERROR_;
}

Error::operator bool() const
{
    return isError();
}

void Error::check(const std::string & message)
{
    Error error = Error::get();

    if (!error)
        return;

    debugmessageoutput::signalError(error, message);
}

} // namespace glow
