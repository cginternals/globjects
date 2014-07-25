#include "ShadingLanguageIncludeImplementation.h"

#include <glbinding/gl/functions.h>

#include <glow/Shader.h>

namespace glow {

std::vector<const char*> ShadingLanguageIncludeImplementation::getSources(const Shader* shader) const
{
    return collectCStrings(shader->source()->strings());
}

void ShadingLanguageIncludeImplementation::compile(const Shader* shader) const
{
    std::vector<const char*> cStrings = collectCStrings(shader->includePaths());
    gl::glCompileShaderIncludeARB(shader->id(), static_cast<gl::GLint>(cStrings.size()), cStrings.data(), nullptr);
}

} // namespace glow
