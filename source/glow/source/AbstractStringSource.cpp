#include <glow/AbstractStringSource.h>

namespace glow
{

std::vector<std::string> AbstractStringSource::strings() const
{
    std::vector<std::string> stringList;
    stringList.push_back(string());
    return stringList;
}

std::vector<AbstractStringSource*> AbstractStringSource::flatten() const
{
    std::vector<AbstractStringSource*> list;

    flattenInto(list);

    return list;
}

std::string AbstractStringSource::shortInfo() const
{
    return "";
}

void AbstractStringSource::flattenInto(std::vector<AbstractStringSource*>& vector) const
{
    vector.push_back(const_cast<AbstractStringSource*>(this));
}

} // namespace glow
