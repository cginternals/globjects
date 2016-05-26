
#include "TextureStorageImplementation_Legacy.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>


namespace globjects
{


TextureStorageImplementation_Legacy::TextureStorageImplementation_Legacy()
{
}

TextureStorageImplementation_Legacy::~TextureStorageImplementation_Legacy()
{
}

void TextureStorageImplementation_Legacy::storage1D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width) const
{
    texture->bind();

    gl::glTexStorage1D(texture->target(), levels, internalFormat, width);
}

void TextureStorageImplementation_Legacy::storage2D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const
{
    texture->bind();

    gl::glTexStorage2D(texture->target(), levels, internalFormat, width, height);
}

void TextureStorageImplementation_Legacy::storage3D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth) const
{
    texture->bind();

    gl::glTexStorage3D(texture->target(), levels, internalFormat, width, height, depth);
}

void TextureStorageImplementation_Legacy::cubeMapStorage(const Texture * texture, gl::GLint levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const
{
    storage2D(texture, levels, internalFormat, width, height);
}


} // namespace globjects
