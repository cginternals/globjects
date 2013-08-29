
#include <cassert>
#include <sstream>
#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
#include <stdexcept>
#endif

#include <glow/logging.h>
#include <glow/Error.h>

#ifdef WIN32
#include <GL/wglew.h>
#elif __APPLE__
#else
#include <GL/glxew.h>
#endif

#ifndef _MSC_VER
#	define APIENTRY
#endif

namespace glow
{

bool Error::s_checking(true);
std::unordered_map<int, int> Error::s_userParamsByContextID;


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

void APIENTRY callback(
    GLenum source
,   GLenum type
,   GLuint id
,   GLenum severity
,   GLsizei length
,   const char * message
,   void * param)
{
    std::stringstream output;

    output << "(id " << id << ", context " << *(reinterpret_cast<int*>(param)) << ", ";

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_ARB:
        output << "high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB:
        output << "medium"; break;
    case GL_DEBUG_SEVERITY_LOW_ARB:
        output << "low"; break;
    }

    output << " severity) ";

    switch (source)
    {
    case GL_DEBUG_SOURCE_API_ARB:
        output << "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
        output << "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
        output << "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
        output << "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION_ARB:
        output << "Application"; break;
    case GL_DEBUG_SOURCE_OTHER_ARB:
        output << "Other"; break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_OTHER_ARB:
    case GL_DEBUG_TYPE_ERROR_ARB:
        fatal() << output.str() << ":"                       << std::endl << message; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        debug() << output.str() << " - deprecated behavior:" << std::endl << message; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        warning() << output.str() << " - undefined behavior:" << std::endl << message; break;
    case GL_DEBUG_TYPE_PORTABILITY_ARB:
        debug() << output.str() << " - portability issue:"    << std::endl << message; break;
    case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        debug() << output.str() << " - performance issue:"    << std::endl << message; break;
    }
}

bool Error::setupDebugOutput(
    const bool asynchronous)
{
    if (!GLEW_ARB_debug_output)
        return false;

#ifdef WIN32
    const HGLRC handle = wglGetCurrentContext();
    const int contextID = reinterpret_cast<int>(handle);
#elif __APPLE__

#else
    const GLXContext handle = glXGetCurrentContext();
    const long long contextID = reinterpret_cast<long long>(handle);
#endif

    glDebugMessageCallback(callback, &s_userParamsByContextID[contextID]);
    CheckGLError();
    s_userParamsByContextID[contextID] = contextID;

    glEnable(GL_DEBUG_OUTPUT);
    if (asynchronous)
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    else
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    return true;
}


} // namespace glow
