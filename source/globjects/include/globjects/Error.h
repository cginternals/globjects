#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

namespace globjects
{

/** \brief Wrapper for OpenGL errors.
 
    To access the current Error (which can be none), call get().
    To discard any current error, call clear().
 */
class GLOBJECTS_API Error
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

protected:
	gl::GLenum m_errorCode;
};

} // namespace globjects
