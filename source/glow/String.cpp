
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

} // namespace glow
