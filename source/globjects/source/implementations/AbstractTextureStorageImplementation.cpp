
#include "AbstractTextureStorageImplementation.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>

#include "TextureStorageImplementation_DirectStateAccessARB.h"
#include "TextureStorageImplementation_DirectStateAccessEXT.h"
#include "TextureStorageImplementation_Legacy.h"
#include "TextureStorageImplementation_Fallback.h"


using namespace gl;

namespace globjects 
{

AbstractTextureStorageImplementation::AbstractTextureStorageImplementation()
{
}

AbstractTextureStorageImplementation::~AbstractTextureStorageImplementation()
{
}

AbstractTextureStorageImplementation * AbstractTextureStorageImplementation::get(const Texture::StorageImplementation impl)
{  
    if (!hasExtension(GLextension::GL_ARB_texture_storage))
    {
        return TextureStorageImplementation_Fallback::instance();
    }

    if (impl == Texture::StorageImplementation::DirectStateAccessARB
     && hasExtension(GLextension::GL_ARB_direct_state_access))
    {
        return TextureStorageImplementation_DirectStateAccessARB::instance();
    }
    else if (impl >= Texture::StorageImplementation::DirectStateAccessEXT
      && hasExtension(GLextension::GL_EXT_direct_state_access))
    {
        return TextureStorageImplementation_DirectStateAccessEXT::instance();
    }
    else
    {
        return TextureStorageImplementation_Legacy::instance();
    }
}

} // namespace globjects
