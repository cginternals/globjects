
#include "AbstractUniformImplementation.h"

#include <globjects/globjects.h>

#include "UniformImplementation_SeparateShaderObjectsARB.h"
#include "UniformImplementation_Legacy.h"


using namespace gl;

namespace globjects 
{

AbstractUniformImplementation::AbstractUniformImplementation()
{
}

AbstractUniformImplementation::~AbstractUniformImplementation()
{
}

AbstractUniformImplementation * AbstractUniformImplementation::get(const AbstractUniform::BindlessImplementation impl)
{
    if (impl == AbstractUniform::BindlessImplementation::SeparateShaderObjectsARB 
     && hasExtension(GLextension::GL_ARB_separate_shader_objects))
    {
        return UniformImplementation_SeparateShaderObjectsARB::instance();
    }
    else
    {
        return UniformImplementation_Legacy::instance();
    }
}

} // namespace globjects
