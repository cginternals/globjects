#include "AbstractBufferImplementation.h"

#include <globjects/globjects.h>

#include "LegacyBufferImplementation.h"
#include "BindlessBufferImplementation.h"

namespace glo {

AbstractBufferImplementation::AbstractBufferImplementation()
{
}

AbstractBufferImplementation::~AbstractBufferImplementation()
{
}

AbstractBufferImplementation * AbstractBufferImplementation::create()
{
    if (hasExtension(gl::GLextension::GL_EXT_direct_state_access))
    {
        return new BindlessBufferImplementation();
    }
    else
    {
        return new LegacyBufferImplementation();
    }
}

} // namespace glo
