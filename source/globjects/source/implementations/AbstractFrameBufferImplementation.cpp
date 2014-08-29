
#include "AbstractFrameBufferImplementation.h"

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include "FramebufferImplementation_DirectStateAccessARB.h"
#include "FramebufferImplementation_DirectStateAccessEXT.h"
#include "FramebufferImplementation_Legacy.h"


using namespace gl;

namespace glo 
{
    
GLenum AbstractFramebufferImplementation::s_workingTarget = GL_FRAMEBUFFER;

AbstractFramebufferImplementation::AbstractFramebufferImplementation()
{
}

AbstractFramebufferImplementation::~AbstractFramebufferImplementation()
{
}

AbstractFramebufferImplementation * AbstractFramebufferImplementation::get(const FrameBufferObject::BindlessImplementation impl)
{
    if (impl == FrameBufferObject::BindlessImplementation::DirectStateAccessARB
     && hasExtension(GLextension::GL_ARB_direct_state_access))
    {
        return FramebufferImplementation_DirectStateAccessARB::instance();
    }
    else if (impl == FrameBufferObject::BindlessImplementation::DirectStateAccessEXT
     && hasExtension(GLextension::GL_EXT_direct_state_access))
    {
        return FramebufferImplementation_DirectStateAccessEXT::instance();
    }
    else
    {
        return FramebufferImplementation_Legacy::instance();
    }
}

} // namespace glo
