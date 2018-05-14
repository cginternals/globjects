
#include "TextureStorageImplementation_DirectStateAccessARB.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include "TextureImplementation_Legacy.h"
#include "TextureImplementation_DirectStateAccessEXT.h"

using namespace gl;


namespace globjects
{


TextureStorageImplementation_DirectStateAccessARB::TextureStorageImplementation_DirectStateAccessARB()
{
}

TextureStorageImplementation_DirectStateAccessARB::~TextureStorageImplementation_DirectStateAccessARB()
{
}

void TextureStorageImplementation_DirectStateAccessARB::storage1D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width) const
{
    gl::glTextureStorage1D(texture->id(), levels, internalFormat, width);
}

void TextureStorageImplementation_DirectStateAccessARB::storage2D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const
{
    gl::glTextureStorage2D(texture->id(), levels, internalFormat, width, height);
}

void TextureStorageImplementation_DirectStateAccessARB::storage3D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth) const
{
    gl::glTextureStorage3D(texture->id(), levels, internalFormat, width, height, depth);
}

void TextureStorageImplementation_DirectStateAccessARB::storage2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSampleLocations) const
{
    gl::glTextureStorage2DMultisample(texture->id(), samples, internalFormat, width, height, fixedSampleLocations);
}

void TextureStorageImplementation_DirectStateAccessARB::storage3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSampleLocations) const
{
    gl::glTextureStorage3DMultisample(texture->id(), samples, internalFormat, width, height, depth, fixedSampleLocations);
}


void TextureStorageImplementation_DirectStateAccessARB::cubeMapStorage(const Texture * texture, gl::GLint levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const
{
    storage2D(texture, levels, internalFormat, width, height);
}


} // namespace globjects
