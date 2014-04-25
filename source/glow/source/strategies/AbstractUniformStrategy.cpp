#include "AbstractUniformStrategy.h"

#include <glow/global.h>

#include "BindlessUniformStrategy.h"
#include "BindfulUniformStrategy.h"

namespace glow {

AbstractUniformStrategy::AbstractUniformStrategy()
{
}

AbstractUniformStrategy::~AbstractUniformStrategy()
{
}

AbstractUniformStrategy * AbstractUniformStrategy::create()
{
    if (hasExtension(GLOW_EXT_direct_state_access))
    {
        return new BindlessUniformStrategy();
    }
    else
    {
        return new BindfulUniformStrategy();
    }
}

} // namespace glow
