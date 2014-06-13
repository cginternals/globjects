#include <glowbase/AbstractStringSource.h>

namespace glow
{

std::vector<std::string> AbstractStringSource::strings() const
{
    std::vector<std::string> stringList;
    stringList.push_back(string());
    return stringList;
}

std::vector<const AbstractStringSource*> AbstractStringSource::flatten() const
{
    std::vector<const AbstractStringSource*> list;

    flattenInto(list);

    return list;
}

std::string AbstractStringSource::shortInfo() const
{
    return "";
}

void AbstractStringSource::flattenInto(std::vector<const AbstractStringSource*>& vector) const
{
    vector.push_back(this);
}

} // namespace glow
