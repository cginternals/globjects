#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow.h>

namespace glow
{

class GLOW_API DebugMessage
{
public:
	DebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, const std::string& message);

	const char* severityString() const;
	const char* sourceString() const;
	const char* typeString() const;

	std::string toString() const;
public:
	GLenum source;
	GLenum type;
	GLuint id;
	GLenum severity;
	std::string message;
public:
	static const char* severityString(GLenum severity);
	static const char* sourceString(GLenum source);
	static const char* typeString(GLenum type);
};

} // namespace glow
