#include <glow/Version.h>
#include <glow/query.h>

#include <sstream>

namespace glow {

Version::Version(GLint majorVersion, GLint minorVersion)
: majorVersion(majorVersion)
, minorVersion(minorVersion)
{
}

Version Version::current()
{
	return query::version();
}

bool Version::operator<(const Version& version) const
{
	return majorVersion<version.majorVersion || (majorVersion==version.majorVersion && minorVersion<version.minorVersion);
}

bool Version::operator>(const Version& version) const
{
	return majorVersion>version.majorVersion || (majorVersion==version.majorVersion && minorVersion>version.minorVersion);
}

bool Version::operator==(const Version& version) const
{
	return majorVersion==version.majorVersion && minorVersion==version.minorVersion;
}

bool Version::operator!=(const Version& version) const
{
	return majorVersion!=version.majorVersion || minorVersion!=version.minorVersion;
}

bool Version::operator>=(const Version& version) const
{
	return *this>version || *this==version;
}

bool Version::operator<=(const Version& version) const
{
	return *this<version || *this==version;
}

std::string Version::toString() const
{
	std::stringstream stream;
	stream << majorVersion << "." << minorVersion;
	return stream.str();
}

} // namespace glow

