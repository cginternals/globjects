#include <globjects/base/StaticStringSource.h>

namespace globjects
{

StaticStringSource::StaticStringSource(const std::string & string)
: m_string(string)
{
}

StaticStringSource::StaticStringSource(const char * data, const size_t length)
: m_string(data, length)
{
}

std::string StaticStringSource::shortInfo() const
{
    return "<static string>";
}

std::string StaticStringSource::string() const
{
    return m_string;
}

void StaticStringSource::setString(const std::string & string)
{
    m_string = string;

    changed();
}

} // namespace globjects
