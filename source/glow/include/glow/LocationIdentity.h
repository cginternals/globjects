#pragma once

#include <string>
#include <functional>

#include <glbinding/types.h>

#include <glow/glow_api.h>

namespace glow {

class GLOW_API LocationIdentity
{
public:
    LocationIdentity();
    LocationIdentity(gl::GLint location);
    LocationIdentity(const std::string & name);

    bool isLocation() const;
    bool isName() const;

    gl::GLint location() const;
    const std::string & name() const;

    bool operator==(const LocationIdentity & identity) const;
    bool operator!=(const LocationIdentity & identity) const;

    bool operator<(const LocationIdentity & identity) const;

    std::size_t hash() const;
protected:
    bool m_invalid;
    bool m_hasName;
    gl::GLint m_location;
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

