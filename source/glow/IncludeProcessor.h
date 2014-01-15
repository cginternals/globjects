#pragma once

#include <string>
#include <set>
#include <vector>

#include <glow/glow.h>

namespace glow
{

class StringSource;
class CompositeStringSource;

class IncludeProcessor
{
public:
    virtual ~IncludeProcessor();

    static StringSource* resolveIncludes(const StringSource* source, const std::vector<std::string>& includePaths);
protected:
    std::set<std::string> m_includes;
    std::vector<std::string> m_includePaths;

    IncludeProcessor();

    CompositeStringSource* process(const StringSource* source);
    CompositeStringSource* processComposite(const StringSource* source);

    static std::string expandPath(const std::string& include, const std::string includePath);
};

} // namespace glow
