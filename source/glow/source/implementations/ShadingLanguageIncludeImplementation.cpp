#include "ShadingLanguageIncludeImplementation.h"

#include <glbinding/gl/functions.h>

#include <glow/Shader.h>

namespace glow {

void ShadingLanguageIncludeImplementation::updateSources(const Shader* shader) const
{
    std::vector<std::string> sources;

    if (shader->source())
    {
        sources = shader->source()->strings();
    }

    std::vector<const char*> cStrings = collectCStrings(sources);

    gl::glShaderSource(shader->id(), static_cast<gl::GLint>(cStrings.size()), cStrings.data(), nullptr);
}

void ShadingLanguageIncludeImplementation::compile(const Shader* shader) const
{
    std::vector<const char*> cStrings = collectCStrings(shader->includePaths());
    gl::glCompileShaderIncludeARB(shader->id(), static_cast<gl::GLint>(cStrings.size()), cStrings.data(), nullptr);
}

} // namespace glow
