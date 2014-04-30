#include "AbstractFrameBufferBehavior.h"

#include <glow/global.h>

#include "BindfulFrameBufferBehavior.h"
#include "BindlessFrameBufferBehavior.h"

namespace glow {

AbstractFrameBufferBehavior::AbstractFrameBufferBehavior()
{
}

AbstractFrameBufferBehavior::~AbstractFrameBufferBehavior()
{
}

AbstractFrameBufferBehavior * AbstractFrameBufferBehavior::create()
{
    if (hasExtension(GLOW_EXT_direct_state_access))
    {
        return new BindlessFrameBufferBehavior();
    }
    else
    {
        return new BindfulFrameBufferBehavior();
    }
}

} // namespace glow
