#include "NoShadingLanguageIncludeImplementation.h"

#include <glbinding/gl/functions.h>

#include <glowbase/AbstractStringSource.h>
#include <glowbase/ref_ptr.h>

#include <glow/Shader.h>

#include "../IncludeProcessor.h"

namespace glow {

std::vector<const char*> NoShadingLanguageIncludeImplementation::getSources(const Shader* shader) const
{
    ref_ptr<AbstractStringSource> resolvedSource = IncludeProcessor::resolveIncludes(shader->source(), shader->includePaths());

    return collectCStrings(resolvedSource->strings());
}

void NoShadingLanguageIncludeImplementation::compile(const Shader* shader) const
{
    gl::glCompileShader(shader->id());
}

} // namespace glow
