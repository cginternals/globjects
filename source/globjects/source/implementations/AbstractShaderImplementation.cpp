
#include "AbstractShaderImplementation.h"

#include <globjects/globjects.h>
#include <glbinding/gl/extension.h>

#include "ShaderImplementation_Legacy.h"


using namespace gl;


namespace globjects 
{


AbstractShaderImplementation::AbstractShaderImplementation()
{
}

AbstractShaderImplementation::~AbstractShaderImplementation()
{
}

AbstractShaderImplementation * AbstractShaderImplementation::get(const Shader::Implementation /*impl*/)
{
    return ShaderImplementation_Legacy::instance();
}


} // namespace globjects
