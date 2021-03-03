
#include "AbstractSamplerImplementation.h"

#include <globjects/globjects.h>
#include <glbinding/gl/extension.h>

#include "SamplerImplementation_Legacy.h"


using namespace gl;


namespace globjects 
{


AbstractSamplerImplementation::AbstractSamplerImplementation()
{
}

AbstractSamplerImplementation::~AbstractSamplerImplementation()
{
}

AbstractSamplerImplementation * AbstractSamplerImplementation::get(const Sampler::Implementation /*impl*/)
{
    return SamplerImplementation_Legacy::instance();
}


} // namespace globjects
