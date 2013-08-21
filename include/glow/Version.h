#pragma once

#include <ostream>
#include <string>

#include <glow/glow.h>


namespace glow
{

class LogMessageBuilder;

class GLOW_API Version
{
public:
    Version(
        int majorVersion = 0
    ,   int minorversion = 0);

	static Version current();

    bool operator< (const Version & version) const;
    bool operator> (const Version & version) const;
    bool operator==(const Version & version) const;
    bool operator!=(const Version & version) const;
    bool operator>=(const Version & version) const;
    bool operator<=(const Version & version) const;

	std::string toString() const;

public:
    int majorVersion;
    int minorVersion;
};

std::ostream & operator<<(
    std::ostream & out
,   const Version & version);

LogMessageBuilder & operator<<(
    LogMessageBuilder & out
,   const Version & version);

} // namespace glow
