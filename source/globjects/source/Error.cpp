#include <globjects/Error.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

namespace glo
{

Error::Error(gl::GLenum errorCode)
: m_errorCode(errorCode)
{
}

Error::Error()
: Error(gl::GL_NO_ERROR)
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
    case gl::GL_NO_ERROR:
        return "GL_NO_ERROR";
    case gl::GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case gl::GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case gl::GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case gl::GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case gl::GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    default:
        return "Unknown GLenum.";
    }
}

Error Error::get()
{
    return Error(gl::glGetError());
}

void Error::clear()
{
    while (Error::get());
}

bool Error::isError() const
{
    return m_errorCode != gl::GL_NO_ERROR;
}

Error::operator bool() const
{
    return isError();
}

} // namespace glo
