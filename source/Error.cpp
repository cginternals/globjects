#include <glow/Error.h>

//#include <stdexcept>
#include <glow/logging.h>
#include <sstream>

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

std::string Error::name() const
{
	return errorString(_errorCode);
}

Error Error::current()
{
	return Error(glGetError());
}

void Error::check(const char* file, int line)
{
	Error error = Error::current();

	if (error.isError())
	{
		std::stringstream ss;
		ss.flags(std::ios::hex | std::ios::showbase);
		ss << "OpenGL error " << error.code() << " : " << error.name() << std::endl;
		ss.unsetf(std::ios::hex | std::ios::showbase);
		ss << "in " << file << ":" << line << std::endl;

		critical() << ss.str();
	}
}

void Error::clear()
{
	while (Error::current().isError());
}

bool Error::isError() const
{
	return _errorCode != GL_NO_ERROR;
}

std::string Error::errorString(GLenum errorCode)
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
