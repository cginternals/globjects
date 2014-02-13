#pragma once

#include <string>
#include <functional>

#include <GL/glew.h>

#include <glow/glow.h>

namespace glow {

class GLOW_API LocationIdentity
{
public:
    LocationIdentity();
    LocationIdentity(GLint location);
    LocationIdentity(const std::string & name);

    bool isLocation() const;
    bool isName() const;

    GLint location() const;
    const std::string & name() const;

    bool operator==(const LocationIdentity & identity) const;
    bool operator!=(const LocationIdentity & identity) const;

    bool operator<(const LocationIdentity & identity) const;

    std::size_t hash() const;
protected:
    bool m_invalid;
    bool m_hasName;
    GLint m_location;
    std::string m_name;
};

} // namespace glow

namespace std {

template <>
struct hash<glow::LocationIdentity>
{
    std::size_t operator()(const glow::LocationIdentity & identity) const
    {
        return identity.hash();
    }
};

} // namespace glow

