
#include <glow/Shader.h>

#include <glowutils/CompoundSource.h>
#include <glowutils/ShaderIncludeRegistry.h>


namespace glow
{

CompoundSource::CompoundSource(ShaderSource * source)
: ShaderSourceDecorator(source)
{
}

CompoundSource::~CompoundSource()
{
}

void CompoundSource::update()
{

}

std::set<Shader*> CompoundSource::requiredShaders() const
{

}

void CompoundSource::resolveInclude(const std::string& include, Shader * shader)
{

}

void CompoundSource::propagateFilename(const std::string filename)
{

}

} // namespace glow
