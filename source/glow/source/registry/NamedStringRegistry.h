#pragma once

#include <string>
#include <unordered_map>

namespace glow {

class NamedString;

class NamedStringRegistry
{
public:
    NamedStringRegistry();
    static NamedStringRegistry & current();

    void registerNamedString(NamedString * namedString);
    void deregisterNamedString(NamedString * namedString);

    bool hasNamedString(const std::string & name);
    NamedString * namedString(const std::string & name);

    bool hasNativeSupport();
protected:
    std::unordered_map<std::string, NamedString*> m_namedStrings;
};

} // namespace glow
