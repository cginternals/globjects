#pragma once

#include <GL/glew.h>

#include <string>

#define CHECK_ERROR glow::Error::check(__FILE__, __LINE__);

namespace glow {

class Error
{
public:
	Error();
	Error(GLenum errorCode);

	static Error current();
	static void check(const char* file, int line);

	GLenum code() const;
	std::string name() const;

	bool isError() const;

	static std::string errorString(GLenum errorCode);
protected:
	GLenum _errorCode;
};


} // namespace glow
