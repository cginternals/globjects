#pragma once

#include <GL/glew.h>

#include <glow/glow.h>

#include <string>

namespace glow {

class GLOW_API Version
{
public:
	Version(GLint majorVersion, GLint minorversion);

	static Version current();

	GLint majorVersion;
	GLint minorVersion;

	bool operator<(const Version& version) const;
	bool operator>(const Version& version) const;
	bool operator==(const Version& version) const;
	bool operator!=(const Version& version) const;
	bool operator>=(const Version& version) const;
	bool operator<=(const Version& version) const;

	std::string toString() const;
};

} // namespace glow
