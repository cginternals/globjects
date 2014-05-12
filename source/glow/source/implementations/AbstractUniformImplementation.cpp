#include "AbstractUniformImplementation.h"

#include <glow/glow.h>

#include "BindlessUniformImplementation.h"
#include "LegacyUniformImplementation.h"

namespace glow {

AbstractUniformImplementation::AbstractUniformImplementation()
{
}

AbstractUniformImplementation::~AbstractUniformImplementation()
{
}

AbstractUniformImplementation * AbstractUniformImplementation::create()
{
    if (hasExtension(gl::Extension::EXT_direct_state_access))
    {
        return new BindlessUniformImplementation();
    }
    else
    {
        return new LegacyUniformImplementation();
    }
}

} // namespace glow
