#include <glow/StringSource.h>

namespace glow
{

std::vector<std::string> StringSource::strings() const
{
    std::vector<std::string> stringList;
    stringList.push_back(string());
    return stringList;
}

std::string StringSource::shortInfo() const
{
    return "<static string>";
}

} // namespace glow
