#include <glow/StringSource.h>

namespace glow
{

std::vector<std::string> StringSource::strings() const
{
    std::vector<std::string> stringList;
    stringList.push_back(string());
    return stringList;
}

std::vector<StringSource*> StringSource::flatten() const
{
    std::vector<StringSource*> list;

    flattenInto(list);

    return list;
}

std::string StringSource::shortInfo() const
{
    return "<static string>";
}

void StringSource::flattenInto(std::vector<StringSource*>& vector) const
{
    vector.push_back(const_cast<StringSource*>(this));
}

} // namespace glow
