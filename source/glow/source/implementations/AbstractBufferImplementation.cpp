#include "AbstractBufferImplementation.h"

#include <glow/glow.h>

#include "LegacyBufferImplementation.h"
#include "BindlessBufferImplementation.h"

namespace glow {

AbstractBufferImplementation::AbstractBufferImplementation()
{
}

AbstractBufferImplementation::~AbstractBufferImplementation()
{
}

AbstractBufferImplementation * AbstractBufferImplementation::create()
{
    if (hasExtension(GLOW_EXT_direct_state_access))
    {
        return new BindlessBufferImplementation();
    }
    else
    {
        return new LegacyBufferImplementation();
    }
}

} // namespace glow
