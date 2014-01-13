#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow.h>

namespace glow
{

/** \brief Wrapper for OpenGL errors.
 
    To access the current Error (which can be none), call get().
    To discard any current error, call clear().
 */
class GLOW_API Error
{
public:
	Error();
	Error(GLenum errorCode);

    static Error get();

	static void clear();

	GLenum code() const;
	std::string name() const;

	bool isError() const;
    operator bool() const;

    static const char * errorString(GLenum errorCode);

protected:
	GLenum m_errorCode;
};

#ifdef NDEBUG
#define CheckGLError()
#else
namespace debugmessageoutput { GLOW_API void manualErrorCheck(const char * file, int line); }
#define CheckGLError() glow::debugmessageoutput::manualErrorCheck(__FILE__, __LINE__)
#endif

} // namespace glow
