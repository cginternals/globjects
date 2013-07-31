#pragma once

#include <GL/glew.h>

#include <string>

#include <glow/glow.h>


#define CHECK_ERROR glow::Error::check(__FILE__, __LINE__);

namespace glow {

class GLOW_API Error
{
public:
	Error();
	Error(GLenum errorCode);

	static Error current();
	static void check(const char* file, int line);
	static void clear();

	GLenum code() const;
	std::string name() const;

	bool isError() const;

	static std::string errorString(GLenum errorCode);
protected:
	GLenum _errorCode;
};


} // namespace glow
