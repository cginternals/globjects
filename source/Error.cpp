#include <glow/Error.h>

//#include <stdexcept>
#include <iostream>

using namespace glow;

Error::Error()
: _errorCode(GL_NO_ERROR)
{
}

Error::Error(GLenum errorCode)
: _errorCode(errorCode)
{
}

GLenum Error::code() const
{
	return _errorCode;
}

const char* Error::name() const
{
	return errorString(_errorCode);
}

Error Error::current()
{
	return Error(glGetError());
}

void Error::check(const char* file, const char* line)
{
	Error error = Error::current();

	if (error.isError())
	{
		std::cout << "OpenGL error " << error.code() << " : " << error.name() << std::endl;
		std::cout << "in " << file << ":" << line << std::endl;
	}
}

bool Error::isError() const
{
	return _errorCode != GL_NO_ERROR;
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
			return "unknown";
	}
}
