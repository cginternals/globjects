
#include <cassert>
#include <sstream>
#include <algorithm>
#ifdef GLOW_GL_ERROR_RAISE_EXCEPTION
#include <stdexcept>
#endif

#include <glow/logging.h>
#include <glow/global.h>
#include <glow/Error.h>

#ifdef WIN32
#include <GL/wglew.h>
#elif __APPLE__
#else
#include <GL/glxew.h>
#endif

namespace glow
{

bool Error::s_checking = true;


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
    Error::get();
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
            assert(false);
			return "Unknown GLenum.";
	}
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
#ifdef NDEBUG
    glow::warning() << "Try to change error checking but glow was compiled with No Debug flag";
#endif
    s_checking = enable;
}

void Error::check(const char* file, int line)
{
    if (!s_checking)
        return;

    Error error = Error::get();

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
    }
}

const char* Error::severityString(GLenum severity)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH_ARB:
            return "high";
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:
            return "medium";
        case GL_DEBUG_SEVERITY_LOW_ARB:
            return "low";
        default:
            return "unknown";
    }
}

const char* Error::sourceString(GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API_ARB:
            return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
            return "Window System";
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
            return "Shader Compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
            return "Third Party";
        case GL_DEBUG_SOURCE_APPLICATION_ARB:
            return "Application";
        case GL_DEBUG_SOURCE_OTHER_ARB:
            return "Other";
        default:
            return "Unknown";
    }
}

const char* Error::typeString(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_OTHER_ARB:
            return "other";
        case GL_DEBUG_TYPE_ERROR_ARB:
            return "error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
            return "deprecated behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
            return "undefined behavior";
        case GL_DEBUG_TYPE_PORTABILITY_ARB:
            return "portability";
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:
            return "performance";
        default:
            return "unknown";
    }
}

void APIENTRY Error::debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char * message, void * param)
{
    std::stringstream output;

    output
            << typeString(type)
            << ": " << std::hex << "0x" << id << std::dec
            << " " << severityString(severity) << " severity"
            << " (" << sourceString(source) << ")"
            << std::endl
            << message;

    if (type == GL_DEBUG_TYPE_ERROR_ARB)
    {
        fatal() << output.str();
    }
    else
    {
        debug() << output.str();
    }
}

bool Error::setupDebugOutput(const bool asynchronous)
{
    //if (!glow::extensions::isSupported("GLEW_ARB_debug_output"))
      //  return false;

    glDebugMessageCallback(&Error::debugCallback, nullptr);
    CheckGLError();

    glEnable(GL_DEBUG_OUTPUT);
    if (asynchronous)
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    else
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    return true;
}

} // namespace glow
