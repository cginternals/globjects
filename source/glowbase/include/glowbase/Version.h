#pragma once

#include <string>

#include <glowbase/glowbase_api.h>

namespace glow
{
/** \brief Simplifies OpenGL version validity and comparision.

*/
class GLOWBASE_API Version
{
public:
    Version();
    Version(unsigned int majorVersion, unsigned int minorversion);

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
    unsigned int majorVersion;
    unsigned int minorVersion;
};

} // namespace glow
