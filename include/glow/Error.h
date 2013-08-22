#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow.h>


namespace glow 
{

class GLOW_API Error
{
public:
	Error();
	Error(GLenum errorCode);

	static Error current();
    static bool check(const char* file, int line);
	static void clear();

	GLenum code() const;
	std::string name() const;

	bool isError() const;

	static std::string errorString(GLenum errorCode);
protected:
	GLenum m_errorCode;
};

#ifdef NDEBUG
#define CheckGLError() false
#else
#define CheckGLError() glow::Error::check(__FILE__, __LINE__)
#endif

} // namespace glow
