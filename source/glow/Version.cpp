
#include <sstream>

#include <glow/global.h>
#include <glow/LogMessageBuilder.h>

#include <glow/Version.h>


namespace glow 
{

std::set<Version> Version::s_validVersions = 
{
    { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 4 }, { 1, 5 },
    { 2, 0 }, { 2, 1 },
    { 3, 0 }, { 3, 1 }, { 3, 2 }, { 3, 3 }, 
    { 4, 0 }, { 4, 1 }, { 4, 2 }, { 4, 3 }, { 4, 4 }
};

Version::Version(int majorVersion, int minorVersion)
: majorVersion(majorVersion)
, minorVersion(minorVersion)
{
}

Version Version::current()
{
	return query::version();
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

bool Version::isValid() const
{
    return s_validVersions.find(*this) != s_validVersions.end();
}

Version Version::nearestValidVersion() const
{
    std::set<Version>::iterator iterator = s_validVersions.lower_bound(*this);

    if (iterator == s_validVersions.end())
    {
        return *(--iterator);
    }

    return *iterator;
}

} // namespace glow
