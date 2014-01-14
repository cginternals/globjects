#pragma once

#include <string>
#include <set>

#include <glow/glow.h>

namespace glow 
{

class StringSource;
class CompositeStringSource;

class IncludeProcessor
{
public:
    virtual ~IncludeProcessor();

    static StringSource* resolveIncludes(const StringSource* source);
protected:
    std::set<std::string> m_includes;

    IncludeProcessor();

    CompositeStringSource* process(const StringSource* source);
    CompositeStringSource* processComposite(const StringSource* source);
};

} // namespace glow
