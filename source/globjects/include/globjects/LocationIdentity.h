#pragma once

#include <string>
#include <functional>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

namespace glo {

class GLOBJECTS_API LocationIdentity
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

} // namespace glo

namespace std {

template <>
struct hash<glo::LocationIdentity>
{
    std::size_t operator()(const glo::LocationIdentity & identity) const
    {
        return identity.hash();
    }
};

} // namespace glo

