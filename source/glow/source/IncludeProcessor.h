#pragma once

#include <string>
#include <set>
#include <vector>

#include <glow/glow_api.h>

namespace glow
{

class AbstractStringSource;
class CompositeStringSource;

class IncludeProcessor
{
public:
    virtual ~IncludeProcessor();

    static AbstractStringSource* resolveIncludes(const AbstractStringSource* source, const std::vector<std::string>& includePaths);
protected:
    std::set<std::string> m_includes;
    std::vector<std::string> m_includePaths;

    IncludeProcessor();

    CompositeStringSource* process(const AbstractStringSource* source);
    CompositeStringSource* processComposite(const AbstractStringSource* source);

    static std::string expandPath(const std::string& include, const std::string includePath);
};

} // namespace glow
