#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow_api.h>

namespace glow
{

class GLOW_API DebugMessage
{
public:
	DebugMessage(
        GLenum source
    ,   GLenum type
    ,   GLuint id
    ,   GLenum severity
    ,   const std::string & message);

	DebugMessage(
		GLenum source
		, GLenum type
		, GLuint id
		, GLenum severity
		, const std::string & message
		, const char* file
		, int line);

	const char * severityString() const;
	const char * sourceString() const;
	const char * typeString() const;

	std::string toString() const;

public:
	static const char * severityString(GLenum severity);
	static const char * sourceString(GLenum source);
	static const char * typeString(GLenum type);

public:
	GLenum source;
	GLenum type;
	GLuint id;
	GLenum severity;
	std::string message;
    const char* file;
    int line;
};

} // namespace glow
