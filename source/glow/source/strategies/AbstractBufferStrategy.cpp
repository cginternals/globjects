#include "AbstractBufferStrategy.h"

#include <glow/global.h>

#include "BindfulBufferStrategy.h"
#include "BindlessBufferStrategy.h"

namespace glow {

AbstractBufferStrategy::AbstractBufferStrategy()
{
}

AbstractBufferStrategy::~AbstractBufferStrategy()
{
}

AbstractBufferStrategy * AbstractBufferStrategy::create()
{
    if (hasExtension(GLOW_EXT_direct_state_access))
    {
        return new BindlessBufferStrategy();
    }
    else
    {
        return new BindfulBufferStrategy();
    }
}

} // namespace glow
