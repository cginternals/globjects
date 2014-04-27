#include "AbstractUniformBehavior.h"

#include <glow/global.h>

#include "BindlessUniformBehavior.h"
#include "BindfulUniformBehavior.h"

namespace glow {

AbstractUniformBehavior::AbstractUniformBehavior()
{
}

AbstractUniformBehavior::~AbstractUniformBehavior()
{
}

AbstractUniformBehavior * AbstractUniformBehavior::create()
{
    if (hasExtension(GLOW_EXT_direct_state_access))
    {
        return new BindlessUniformBehavior();
    }
    else
    {
        return new BindfulUniformBehavior();
    }
}

} // namespace glow
