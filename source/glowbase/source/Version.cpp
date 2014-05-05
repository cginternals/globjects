#include <glowbase/Version.h>

#include <sstream>
#include <set>

namespace {
    static std::set<glow::Version> validVersions =
    {
        { 3, 0 }, { 3, 1 }, { 3, 2 }, { 3, 3 },
        { 4, 0 }, { 4, 1 }, { 4, 2 }, { 4, 3 }, { 4, 4 }
    };
}


namespace glow 
{

Version::Version()
: majorVersion(0)
, minorVersion(0)
{
}

Version::Version(unsigned int majorVersion, unsigned int minorVersion)
: majorVersion(majorVersion)
, minorVersion(minorVersion)
{
}

bool Version::operator<(const Version & version) const
{
    return majorVersion < version.majorVersion
        || (majorVersion == version.majorVersion && minorVersion < version.minorVersion);
}

bool Version::operator>(const Version & version) const
{
    return majorVersion > version.majorVersion
        || (majorVersion == version.majorVersion && minorVersion>version.minorVersion);
}

bool Version::operator==(const Version & version) const
{
    return majorVersion == version.majorVersion
        && minorVersion == version.minorVersion;
}

bool Version::operator!=(const Version & version) const
{
    return majorVersion != version.majorVersion
        || minorVersion != version.minorVersion;
}

bool Version::operator>=(const Version & version) const
{
    return *this > version || *this == version;
}

bool Version::operator<=(const Version & version) const
{
    return *this < version || *this == version;
}

std::string Version::toString() const
{
	std::stringstream stream;
	stream << majorVersion << "." << minorVersion;

    return stream.str();
}

bool Version::isNull() const
{
    return majorVersion == 0 && minorVersion == 0;
}

bool Version::isValid() const
{
    return validVersions.find(*this) != validVersions.end();
}

Version Version::nearestValidVersion() const
{
    std::set<Version>::iterator iterator = validVersions.lower_bound(*this);

    if (iterator == validVersions.end())
    {
        return *(--iterator);
    }

    return *iterator;
}

} // namespace glow
