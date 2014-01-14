
#include <glow/String.h>

namespace glow
{

String::String(const std::string& string)
: m_string(string)
{
}

std::string String::string() const
{
    return m_string;
}

void String::setString(const std::string& string)
{
    m_string = string;

    changed();
}

} // namespace glow
