#include "AbstractUniformImplementation.h"

#include <globjects/globjects.h>

#include "BindlessUniformImplementation.h"
#include "LegacyUniformImplementation.h"

namespace glo {

AbstractUniformImplementation::AbstractUniformImplementation()
{
}

AbstractUniformImplementation::~AbstractUniformImplementation()
{
}

AbstractUniformImplementation * AbstractUniformImplementation::create()
{
    if (hasExtension(gl::GLextension::GL_ARB_separate_shader_objects))
    {
        return new BindlessUniformImplementation();
    }
    else
    {
        return new LegacyUniformImplementation();
    }
}

} // namespace glo
