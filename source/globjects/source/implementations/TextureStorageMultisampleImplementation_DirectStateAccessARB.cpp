
#include "TextureStorageMultisampleImplementation_DirectStateAccessARB.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include "TextureImplementation_Legacy.h"
#include "TextureImplementation_DirectStateAccessEXT.h"


using namespace gl;


namespace globjects
{


TextureStorageMultisampleImplementation_DirectStateAccessARB::TextureStorageMultisampleImplementation_DirectStateAccessARB()
{
}

TextureStorageMultisampleImplementation_DirectStateAccessARB::~TextureStorageMultisampleImplementation_DirectStateAccessARB()
{
}

void TextureStorageMultisampleImplementation_DirectStateAccessARB::storage2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSampleLocations) const
{
    gl::glTextureStorage2DMultisample(texture->id(), samples, internalFormat, width, height, fixedSampleLocations);
}

void TextureStorageMultisampleImplementation_DirectStateAccessARB::storage3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSampleLocations) const
{
    gl::glTextureStorage3DMultisample(texture->id(), samples, internalFormat, width, height, depth, fixedSampleLocations);
}


} // namespace globjects
