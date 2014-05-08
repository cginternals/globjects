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

    static void check(const std::string & message);
protected:
	gl::GLenum m_errorCode;
};

} // namespace glow
