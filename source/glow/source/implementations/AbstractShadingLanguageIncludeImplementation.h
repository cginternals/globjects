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

    virtual std::vector<const char*> getSources(const Shader* shader) const = 0;
    virtual void compile(const Shader* shader) const = 0;

    static std::vector<const char*> collectCStrings(const std::vector<std::string> & strings);
};

} // namespace glow
