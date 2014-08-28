
#include "AbstractBufferImplementation.h"

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include "LegacyBufferImplementation.h"
#include "BindlessBufferImplementation.h"
#include "BindlessEXTBufferImplementation.h"

using namespace gl;

namespace glo {

GLenum AbstractBufferImplementation::s_workingTarget = GL_COPY_WRITE_BUFFER;

AbstractBufferImplementation::AbstractBufferImplementation()
{
}

AbstractBufferImplementation::~AbstractBufferImplementation()
{
}

AbstractBufferImplementation * AbstractBufferImplementation::get()
{
    if (hasExtension(GLextension::GL_ARB_direct_state_access))
    {
        return new BindlessBufferImplementation();
    }
    else if (hasExtension(GLextension::GL_EXT_direct_state_access))
    {
        return new BindlessEXTBufferImplementation();
    }
    else
    {
        return new LegacyBufferImplementation();
    }
}

} // namespace glo
