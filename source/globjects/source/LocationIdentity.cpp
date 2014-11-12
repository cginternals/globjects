#include <globjects/LocationIdentity.h>


using namespace gl;

namespace globjects 
{

LocationIdentity::LocationIdentity()
: m_invalid(true)
, m_location(-1)
, m_hasName(false)
{
}

LocationIdentity::LocationIdentity(const GLint location)
: m_invalid(false)
, m_location(location)
, m_hasName(false)
{
}

LocationIdentity::LocationIdentity(const std::string & name)
: m_invalid(false)
, m_location(-1)
, m_name(name)
, m_hasName(true)
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

} // namespace globjects
