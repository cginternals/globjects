
#include <sstream>

#include <glow/global.h>
#include <glow/LogMessage.h>
#include <glow/Version.h>


namespace glow {

Version::Version(
    int majorVersion
,   int minorVersion)
:   majorVersion(majorVersion)
,   minorVersion(minorVersion)
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

std::ostream & operator<<(
    std::ostream & out
,   const Version & version)
{
    return out << version.toString();
}

LogMessageBuilder & operator<<(
    LogMessageBuilder & out
,   const Version & version)
{
    return out << version.toString();
}

std::string Version::toString() const
{
	std::stringstream stream;
	stream << majorVersion << "." << minorVersion;

    return stream.str();
}

} // namespace glow
