#include <glow/Error.h>

#include <glow/debugmessageoutput.h>

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
	return errorString(m_errorCode);
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

const char* Error::errorString(GLenum errorCode)
{
	switch(errorCode)
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

void Error::check(const char *file, int line)
{
    debugmessageoutput::manualErrorCheck(file, line);
}

} // namespace glow
