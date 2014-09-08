
#include "ShadingLanguageIncludeImplementation_Fallback.h"

#include <glbinding/gl/functions.h>

#include <globjects/base/AbstractStringSource.h>
#include <globjects/base/ref_ptr.h>

#include <globjects/Shader.h>

#include "../IncludeProcessor.h"


using namespace gl;

namespace globjects 
{

void ShadingLanguageIncludeImplementation_Fallback::updateSources(const Shader * shader) const
{
    std::vector<std::string> sources;

    if (shader->source())
    {
        ref_ptr<AbstractStringSource> resolvedSource = IncludeProcessor::resolveIncludes(shader->source(), shader->includePaths());

        sources = resolvedSource->strings();
    }

    std::vector<const char *> cStrings = collectCStrings(sources);

    glShaderSource(shader->id(), static_cast<GLint>(cStrings.size()), cStrings.data(), nullptr);
}

void ShadingLanguageIncludeImplementation_Fallback::compile(const Shader * shader) const
{
    glCompileShader(shader->id());
}

} // namespace globjects
