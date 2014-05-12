#include "AbstractFrameBufferImplementation.h"

#include <glow/glow.h>

#include "LegacyFrameBufferImplementation.h"
#include "BindlessFrameBufferImplementation.h"

namespace glow {

AbstractFrameBufferImplementation::AbstractFrameBufferImplementation()
{
}

AbstractFrameBufferImplementation::~AbstractFrameBufferImplementation()
{
}

AbstractFrameBufferImplementation * AbstractFrameBufferImplementation::create()
{
    if (hasExtension(gl::Extension::EXT_direct_state_access))
    {
        return new BindlessFrameBufferImplementation();
    }
    else
    {
        return new LegacyFrameBufferImplementation();
    }
}

} // namespace glow
