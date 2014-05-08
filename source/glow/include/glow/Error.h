#pragma once

#include <string>

#include <glbinding/types.h>

#include <glow/glow_api.h>

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
	Error(gl::GLenum errorCode);

    static Error get();

	static void clear();

	gl::GLenum code() const;
	std::string name() const;

	bool isError() const;
    operator bool() const;

    static void check(const char * file, int line);
protected:
	gl::GLenum m_errorCode;
};

#ifdef NDEBUG
#define CheckGLError()
#else
#define CheckGLError() glow::Error::check(__FILE__, __LINE__)
#endif

} // namespace glow
