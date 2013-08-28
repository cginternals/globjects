
#include <cassert>
#include <sstream>
#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
#include <stdexcept>
#endif

#include <glow/logging.h>
#include <glow/Error.h>

namespace glow
{

bool Error::s_checking(true);

Error::Error(GLenum errorCode)
:   m_errorCode(errorCode)
{
}

Error::Error()
:   Error(GL_NO_ERROR)
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

Error Error::current()
{
	return Error(glGetError());
}

bool Error::isChecking()
{
#ifdef NDEBUG
    return false;
#else
    return s_checking;
#endif
}

void Error::setChecking(const bool enable)
{
    s_checking = enable;
}

bool Error::get(const char* file, int line)
{
    if (!s_checking)
        return false;

	Error error = Error::current();

	if (error.isError())
	{
		std::stringstream ss;
		ss.flags(std::ios::hex | std::ios::showbase);
        ss << "OpenGL " << error.name() << " (" << error.code() << ")";
		ss.unsetf(std::ios::hex | std::ios::showbase);
		ss << " in " << file << "(" << line << ")";

#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
		throw std::runtime_error(ss.str());
#else
		critical() << ss.str();
#endif
        return true;
	}
    return false;
}

void Error::clear()
{
	while (Error::current().isError());
}

bool Error::isError() const
{
	return m_errorCode != GL_NO_ERROR;
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
            assert(false);
			return "Unknown GLenum.";
	}
}


// ARB_debug_output

bool  Error::setupDebugOutput(
    const bool asynchronous)
{
    return false;
}

void Error::callback(
    GLenum source
,   GLenum type
,   GLuint id
,   GLenum severity
,   GLsizei length
,   const char * message
,   void * param)
{
    fatal() << message;
}


} // namespace glow
