#include <glow/LocationIdentity.h>

namespace glow {

LocationIdentity::LocationIdentity()
: m_invalid(true)
{
}

LocationIdentity::LocationIdentity(GLint location)
: m_invalid(false)
, m_hasName(false)
, m_location(location)
{
}

LocationIdentity::LocationIdentity(const std::string & name)
: m_invalid(false)
, m_hasName(true)
, m_location(-1)
, m_name(name)
{
}

bool LocationIdentity::isLocation() const
{
    return !m_hasName && !m_invalid;
}

bool LocationIdentity::isName() const
{
    return m_hasName && !m_invalid;
}

GLint LocationIdentity::location() const
{
    return m_location;
}

const std::string & LocationIdentity::name() const
{
    return m_name;
}

bool LocationIdentity::operator==(const LocationIdentity & identity) const
{
    if (m_invalid)
        return identity.m_invalid;

    if (identity.m_invalid)
        return false;

    if (m_hasName != identity.m_hasName)
        return false;

    if (m_hasName)
        return m_name == identity.m_name;

    return m_location == identity.m_location;
}

bool LocationIdentity::operator!=(const LocationIdentity & identity) const
{
    return !(*this == identity);
}

bool LocationIdentity::operator<(const LocationIdentity & identity) const
{
    if (m_invalid || identity.m_invalid)
        return false;

    if (m_hasName != identity.m_hasName)
        return !m_hasName; // locations before names

    if (m_hasName)
        return m_name < identity.m_name;

    return m_location < identity.m_location;
}

std::size_t LocationIdentity::hash() const
{
    if (m_hasName)
    {
        return std::hash<std::string>()(m_name);
    }
    else
    {
        return std::hash<GLint>()(m_location);
    }
}

} // namespace glow
