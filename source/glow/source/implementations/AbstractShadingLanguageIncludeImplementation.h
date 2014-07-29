#pragma once

#include <string>
#include <vector>

namespace glow
{

class Shader;

class AbstractShadingLanguageIncludeImplementation
{
public:
    AbstractShadingLanguageIncludeImplementation();
    virtual ~AbstractShadingLanguageIncludeImplementation();

    static AbstractShadingLanguageIncludeImplementation * create();

    virtual void updateSources(const Shader* shader) const = 0;
    virtual void compile(const Shader* shader) const = 0;

    static std::vector<const char*> collectCStrings(const std::vector<std::string> & strings);

    static bool forceFallbackIncludeProcessor;
};

} // namespace glow
