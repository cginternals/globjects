#include "AbstractBufferImplementation.h"

#include <globjects/globjects.h>

#include "LegacyBufferImplementation.h"
#include "BindlessBufferImplementation.h"
#include "BindlessEXTBufferImplementation.h"

namespace glo {

AbstractBufferImplementation::AbstractBufferImplementation()
{
}

AbstractBufferImplementation::~AbstractBufferImplementation()
{
}

AbstractBufferImplementation * AbstractBufferImplementation::create()
{
    if (hasExtension(gl::GLextension::GL_ARB_direct_state_access))
    {
        return new BindlessBufferImplementation();
    }
    else if (hasExtension(gl::GLextension::GL_EXT_direct_state_access))
    {
        return new BindlessEXTBufferImplementation();
    }
    else
    {
        return new LegacyBufferImplementation();
    }
}

} // namespace glo
