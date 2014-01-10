
#include <sstream>

#include <glow/global.h>
#include <glow/LogMessageBuilder.h>

#include <glow/Version.h>


namespace glow 
{

std::set<Version> Version::s_validVersions = 
{
    { 3, 0 }, { 3, 1 }, { 3, 2 }, { 3, 3 }, 
    { 4, 0 }, { 4, 1 }, { 4, 2 }, { 4, 3 }, { 4, 4 }
};

Version::Version()
: majorVersion(0)
, minorVersion(0)
{
}

Version::Version(int majorVersion, int minorVersion)
: majorVersion(majorVersion)
, minorVersion(minorVersion)
{
}

Version Version::current()
{
    return Version(currentMajorVersion(), currentMinorVersion());
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

std::string Version::vendor()
{
    return getString(GL_VENDOR);
}

std::string Version::renderer()
{
    return getString(GL_RENDERER);
}

std::string Version::versionString()
{
    return getString(GL_VERSION);
}

GLint Version::currentMajorVersion()
{
    return getInteger(GL_MAJOR_VERSION);
}

GLint Version::currentMinorVersion()
{
    return getInteger(GL_MINOR_VERSION);
}

bool Version::currentVersionIsInCoreProfile()
{
    if (current()<Version(3,2))
    {
        return false;
    }

    return (getInteger(GL_CONTEXT_PROFILE_MASK) & GL_CONTEXT_CORE_PROFILE_BIT) > 0;
}

} // namespace glow
