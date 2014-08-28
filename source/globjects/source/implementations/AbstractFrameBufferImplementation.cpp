
#include "AbstractFrameBufferImplementation.h"

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include "LegacyFrameBufferImplementation.h"
#include "BindlessFrameBufferImplementation.h"
#include "BindlessEXTFrameBufferImplementation.h"


using namespace gl;

namespace glo 
{
    
GLenum AbstractFrameBufferImplementation::s_workingTarget = GL_FRAMEBUFFER;

AbstractFrameBufferImplementation::AbstractFrameBufferImplementation()
{
}

AbstractFrameBufferImplementation::~AbstractFrameBufferImplementation()
{
}

AbstractFrameBufferImplementation * AbstractFrameBufferImplementation::get()
{
    if (hasExtension(GLextension::GL_ARB_direct_state_access))
    {
        return new BindlessFrameBufferImplementation();
    }
    else if (hasExtension(GLextension::GL_EXT_direct_state_access))
    {
        return new BindlessEXTFrameBufferImplementation();
    }
    else
    {
        return new LegacyFrameBufferImplementation();
    }
}

} // namespace glo
