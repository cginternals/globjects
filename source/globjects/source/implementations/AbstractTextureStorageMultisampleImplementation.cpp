
#include "AbstractTextureStorageMultisampleImplementation.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>

#include "TextureStorageMultisampleImplementation_DirectStateAccessARB.h"
#include "TextureStorageMultisampleImplementation_DirectStateAccessEXT.h"
#include "TextureStorageMultisampleImplementation_Legacy.h"
#include "TextureStorageMultisampleImplementation_Fallback.h"


using namespace gl;


namespace globjects
{


AbstractTextureStorageMultisampleImplementation::AbstractTextureStorageMultisampleImplementation()
{
}

AbstractTextureStorageMultisampleImplementation::~AbstractTextureStorageMultisampleImplementation()
{
}

AbstractTextureStorageMultisampleImplementation * AbstractTextureStorageMultisampleImplementation::get(const Texture::StorageMultisampleImplementation impl)
{
    if (!hasExtension(GLextension::GL_ARB_texture_storage_multisample))
    {
        return TextureStorageMultisampleImplementation_Fallback::instance();
    }

    if (impl == Texture::StorageMultisampleImplementation::DirectStateAccessARB
     && hasExtension(GLextension::GL_ARB_direct_state_access))
    {
        return TextureStorageMultisampleImplementation_DirectStateAccessARB::instance();
    }
    else if (impl >= Texture::StorageMultisampleImplementation::DirectStateAccessEXT
      && hasExtension(GLextension::GL_EXT_direct_state_access))
    {
        return TextureStorageMultisampleImplementation_DirectStateAccessEXT::instance();
    }
    else
    {
        return TextureStorageMultisampleImplementation_Legacy::instance();
    }
}


} // namespace globjects
