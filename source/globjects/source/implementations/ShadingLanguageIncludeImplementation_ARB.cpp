
#include "ShadingLanguageIncludeImplementation_ARB.h"

#include <glbinding/gl/functions.h>

#include <globjects/Shader.h>

#include <globjects/base/AbstractStringSource.h>


using namespace gl;

namespace globjects 
{

void ShadingLanguageIncludeImplementation_ARB::updateSources(const Shader * shader) const
{
    std::string source;
    if (shader->source()) {
        source = shader->source()->string();
    }

    const char* sourcePtr = source.c_str();
    glShaderSource(shader->id(), static_cast<GLint>(1), &sourcePtr, nullptr);
}

void ShadingLanguageIncludeImplementation_ARB::compile(const Shader * shader) const
{
    std::vector<const char*> cStrings = collectCStrings(shader->includePaths());
    glCompileShaderIncludeARB(shader->id(), static_cast<GLint>(cStrings.size()), cStrings.data(), nullptr);
}

} // namespace globjects
