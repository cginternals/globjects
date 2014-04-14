#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow_api.h>


namespace glow
{
/** \brief Simplifies OpenGL version validity and comparision.

*/
class GLOW_API Version
{
public:
    Version();
    Version(GLint majorVersion, GLint minorversion);

	static Version current();

    bool operator< (const Version & version) const;
    bool operator> (const Version & version) const;
    bool operator==(const Version & version) const;
    bool operator!=(const Version & version) const;
    bool operator>=(const Version & version) const;
    bool operator<=(const Version & version) const;

	std::string toString() const;

    bool isValid() const;
    bool isNull() const;

    Version nearestValidVersion() const;
public:
    GLint majorVersion;
    GLint minorVersion;
};

} // namespace glow
