#pragma once

#include <string>
#include <vector>

#include <globjects/Shader.h>


namespace globjects
{

class Shader;

class AbstractShadingLanguageIncludeImplementation
{
public:
    AbstractShadingLanguageIncludeImplementation();
    virtual ~AbstractShadingLanguageIncludeImplementation();

    static AbstractShadingLanguageIncludeImplementation * get(Shader::IncludeImplementation impl = 
        Shader::IncludeImplementation::ShadingLanguageIncludeARB);

    virtual void updateSources(const Shader * shader) const = 0;
    virtual void compile(const Shader * shader) const = 0;

    static std::vector<const char*> collectCStrings(const std::vector<std::string> & strings);
};

} // namespace globjects
