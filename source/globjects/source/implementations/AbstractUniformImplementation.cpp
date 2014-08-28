
#include "AbstractUniformImplementation.h"

#include <globjects/globjects.h>

#include "BindlessUniformImplementation.h"
#include "LegacyUniformImplementation.h"


using namespace gl;

namespace glo 
{

AbstractUniformImplementation::AbstractUniformImplementation()
{
}

AbstractUniformImplementation::~AbstractUniformImplementation()
{
}

AbstractUniformImplementation * AbstractUniformImplementation::get()
{
    if (hasExtension(GLextension::GL_ARB_separate_shader_objects))
    {
        return new BindlessUniformImplementation();
    }
    else
    {
        return new LegacyUniformImplementation();
    }
}

} // namespace glo
