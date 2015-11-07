
#include "AbstractUniformImplementation.h"

#include <glbinding/gl/extension.h>

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

void AbstractUniformImplementation::set(const Program* program, GLint location, const std::vector<TextureHandle> & value) const
{
    set(program, location, std::vector<GLuint64>{ value.begin(), value.end() });
}

} // namespace globjects
