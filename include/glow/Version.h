#pragma once

#include <ostream>
#include <string>
#include <set>

#include <glow/glow.h>


namespace glow
{
/** \brief Simplifies OpenGL version validity and comparision.

*/
class GLOW_API Version
{
public:
    Version();
    Version(int majorVersion, int minorversion = 0);

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
    int majorVersion;
    int minorVersion;

private:
    static std::set<Version> s_validVersions;
};

} // namespace glow
