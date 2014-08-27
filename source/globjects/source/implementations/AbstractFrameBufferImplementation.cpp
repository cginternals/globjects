#include "AbstractFrameBufferImplementation.h"

#include <globjects/globjects.h>

#include "LegacyFrameBufferImplementation.h"
#include "BindlessFrameBufferImplementation.h"
#include "BindlessEXTFrameBufferImplementation.h"

namespace glo {

AbstractFrameBufferImplementation::AbstractFrameBufferImplementation()
{
}

AbstractFrameBufferImplementation::~AbstractFrameBufferImplementation()
{
}

AbstractFrameBufferImplementation * AbstractFrameBufferImplementation::create()
{
    if (hasExtension(gl::GLextension::GL_ARB_direct_state_access))
    {
        return new BindlessFrameBufferImplementation();
    }
    else if (hasExtension(gl::GLextension::GL_EXT_direct_state_access))
    {
        return new BindlessEXTFrameBufferImplementation();
    }
    else
    {
        return new LegacyFrameBufferImplementation();
    }
}

} // namespace glo
