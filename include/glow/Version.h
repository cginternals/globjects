#pragma once

#include <ostream>
#include <string>
#include <set>

#include <GL/glew.h>

#include <glow/glow.h>


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

    static std::string vendor();
    static std::string renderer();

    static Version version();
    static std::string versionString();

    static GLint currentMajorVersion();
    static GLint currentMinorVersion();
    static bool currentVersionIsInCoreProfile();

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

private:
    static std::set<Version> s_validVersions;
};

} // namespace glow
