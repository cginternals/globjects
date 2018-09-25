
#include "TextureStorageMultisampleImplementation_Legacy.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>


namespace globjects
{


TextureStorageMultisampleImplementation_Legacy::TextureStorageMultisampleImplementation_Legacy()
{
}

TextureStorageMultisampleImplementation_Legacy::~TextureStorageMultisampleImplementation_Legacy()
{
}

void TextureStorageMultisampleImplementation_Legacy::storage2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSampleLocations) const
{
    texture->bind();

    gl::glTexStorage2DMultisample(texture->target(), samples, internalFormat, width, height, fixedSampleLocations);
}

void TextureStorageMultisampleImplementation_Legacy::storage3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSampleLocations) const
{
    texture->bind();

    gl::glTexStorage3DMultisample(texture->target(), samples, internalFormat, width, height, depth, fixedSampleLocations);
}


} // namespace globjects
