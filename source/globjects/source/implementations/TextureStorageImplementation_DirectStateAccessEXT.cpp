
#include "TextureStorageImplementation_DirectStateAccessEXT.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>


using namespace gl;


namespace globjects
{


TextureStorageImplementation_DirectStateAccessEXT::TextureStorageImplementation_DirectStateAccessEXT()
{
}

TextureStorageImplementation_DirectStateAccessEXT::~TextureStorageImplementation_DirectStateAccessEXT()
{
}

void TextureStorageImplementation_DirectStateAccessEXT::storage1D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width) const
{
    gl::glTextureStorage1DEXT(texture->id(), texture->target(), levels, internalFormat, width);
}

void TextureStorageImplementation_DirectStateAccessEXT::storage2D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const
{
    gl::glTextureStorage2DEXT(texture->id(), texture->target(), levels, internalFormat, width, height);
}

void TextureStorageImplementation_DirectStateAccessEXT::storage3D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth) const
{
    gl::glTextureStorage3DEXT(texture->id(), texture->target(), levels, internalFormat, width, height, depth);
}

void TextureStorageImplementation_DirectStateAccessEXT::cubeMapStorage(const Texture * texture, gl::GLint levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const
{
    storage2D(texture, levels, internalFormat, width, height);
}


} // namespace globjects
