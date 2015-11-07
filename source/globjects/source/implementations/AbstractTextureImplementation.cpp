
#include "AbstractTextureImplementation.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>

#include "TextureImplementation_DirectStateAccessARB.h"
#include "TextureImplementation_DirectStateAccessEXT.h"
#include "TextureImplementation_Legacy.h"


using namespace gl;

namespace globjects 
{

AbstractTextureImplementation::AbstractTextureImplementation()
{
}

AbstractTextureImplementation::~AbstractTextureImplementation()
{
}

AbstractTextureImplementation * AbstractTextureImplementation::get(const Texture::BindlessImplementation impl)
{  
    if (impl == Texture::BindlessImplementation::DirectStateAccessARB
     && hasExtension(GLextension::GL_ARB_direct_state_access))
    {
        return TextureImplementation_DirectStateAccessARB::instance();
    }
    else if (impl >= Texture::BindlessImplementation::DirectStateAccessEXT
      && hasExtension(GLextension::GL_EXT_direct_state_access))
    {
        return TextureImplementation_DirectStateAccessEXT::instance();
    }
    else
    {
        return TextureImplementation_Legacy::instance();
    }
}

} // namespace globjects
