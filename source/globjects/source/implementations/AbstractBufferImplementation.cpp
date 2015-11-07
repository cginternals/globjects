
#include "AbstractBufferImplementation.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>

#include "BufferImplementation_DirectStateAccessARB.h"
#include "BufferImplementation_DirectStateAccessEXT.h"
#include "BufferImplementation_Legacy.h"


using namespace gl;

namespace globjects 
{

AbstractBufferImplementation::AbstractBufferImplementation()
{
}

AbstractBufferImplementation::~AbstractBufferImplementation()
{
}

AbstractBufferImplementation * AbstractBufferImplementation::get(const Buffer::BindlessImplementation impl)
{  
    if (impl == Buffer::BindlessImplementation::DirectStateAccessARB 
     && hasExtension(GLextension::GL_ARB_direct_state_access))
    {
        return BufferImplementation_DirectStateAccessARB::instance();
    }
    else if (impl >= Buffer::BindlessImplementation::DirectStateAccessEXT 
      && hasExtension(GLextension::GL_EXT_direct_state_access))
    {
        return BufferImplementation_DirectStateAccessEXT::instance();
    }
    else
    {
        return BufferImplementation_Legacy::instance();
    }
}

} // namespace globjects
