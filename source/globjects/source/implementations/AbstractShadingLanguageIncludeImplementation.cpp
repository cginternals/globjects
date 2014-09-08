
#include "AbstractShadingLanguageIncludeImplementation.h"

#include <globjects/globjects.h>

#include <glbinding/gl/extension.h>

#include "ShadingLanguageIncludeImplementation_ARB.h"
#include "ShadingLanguageIncludeImplementation_Fallback.h"


using namespace gl;

namespace globjects 
{

AbstractShadingLanguageIncludeImplementation::AbstractShadingLanguageIncludeImplementation()
{
}

AbstractShadingLanguageIncludeImplementation::~AbstractShadingLanguageIncludeImplementation()
{
}

AbstractShadingLanguageIncludeImplementation * AbstractShadingLanguageIncludeImplementation::get(const Shader::IncludeImplementation impl)
{
    if (impl == Shader::IncludeImplementation::ShadingLanguageIncludeARB 
     && hasExtension(GLextension::GL_ARB_shading_language_include))
    {
        return ShadingLanguageIncludeImplementation_ARB::instance();
    }
    else
    {
        return ShadingLanguageIncludeImplementation_Fallback::instance();
    }
}

std::vector<const char *> AbstractShadingLanguageIncludeImplementation::collectCStrings(const std::vector<std::string> & strings)
{
    std::vector<const char *> cStrings;

    for (const std::string & str : strings)
    {
        cStrings.push_back(str.c_str());
    }

    return cStrings;
}

} // namespace globjects
