
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

void CompoundSource::resolveInclude(const std::string& pathname, ShaderSource * shaderSource)
{

}

void CompoundSource::propagateFilename(const std::string& filename)
{

}

} // namespace glow
