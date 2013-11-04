#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow.h>

#include <glow/DebugMessageOutput.h>

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

} // namespace glow
