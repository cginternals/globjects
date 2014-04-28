#include "AbstractBufferBehavior.h"

#include <glow/glow.h>

#include "BindfulBufferBehavior.h"
#include "BindlessBufferBehavior.h"

namespace glow {

AbstractBufferBehavior::AbstractBufferBehavior()
{
}

AbstractBufferBehavior::~AbstractBufferBehavior()
{
}

AbstractBufferBehavior * AbstractBufferBehavior::create()
{
    if (hasExtension(GLOW_EXT_direct_state_access))
    {
        return new BindlessBufferBehavior();
    }
    else
    {
        return new BindfulBufferBehavior();
    }
}

} // namespace glow
