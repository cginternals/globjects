#include "AbstractShadingLanguageIncludeImplementation.h"

#include <glow/glow.h>

#include <glbinding/gl/extension.h>

#include "NoShadingLanguageIncludeImplementation.h"
#include "ShadingLanguageIncludeImplementation.h"

namespace glow {

AbstractShadingLanguageIncludeImplementation::AbstractShadingLanguageIncludeImplementation()
{
}

AbstractShadingLanguageIncludeImplementation::~AbstractShadingLanguageIncludeImplementation()
{
}

AbstractShadingLanguageIncludeImplementation * AbstractShadingLanguageIncludeImplementation::create()
{
    if (hasExtension(gl::GLextension::GL_ARB_shading_language_include))
    {
        return new ShadingLanguageIncludeImplementation();
    }
    else
    {
        return new NoShadingLanguageIncludeImplementation();
    }
}

std::vector<const char*> AbstractShadingLanguageIncludeImplementation::collectCStrings(const std::vector<std::string> & strings)
{
    std::vector<const char*> cStrings;

    for (const std::string & str : strings)
    {
        cStrings.push_back(str.c_str());
    }

    return cStrings;
}

} // namespace glow