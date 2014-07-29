#include "NoShadingLanguageIncludeImplementation.h"

#include <glbinding/gl/functions.h>

#include <glowbase/AbstractStringSource.h>
#include <glowbase/ref_ptr.h>

#include <glow/Shader.h>

#include "../IncludeProcessor.h"

namespace glow {

void NoShadingLanguageIncludeImplementation::updateSources(const Shader* shader) const
{
    std::vector<std::string> sources;

    if (shader->source())
    {
        ref_ptr<AbstractStringSource> resolvedSource = IncludeProcessor::resolveIncludes(shader->source(), shader->includePaths());

        sources = resolvedSource->strings();
    }

    std::vector<const char*> cStrings = collectCStrings(sources);

    gl::glShaderSource(shader->id(), static_cast<gl::GLint>(cStrings.size()), cStrings.data(), nullptr);
}

void NoShadingLanguageIncludeImplementation::compile(const Shader* shader) const
{
    gl::glCompileShader(shader->id());
}

} // namespace glow
