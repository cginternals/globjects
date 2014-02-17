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

    static void check(const char * file, int line);
protected:
	GLenum m_errorCode;
};

#ifdef NDEBUG
#define CheckGLError()
#else
#define CheckGLError() glow::Error::check(__FILE__, __LINE__)
#endif

} // namespace glow
