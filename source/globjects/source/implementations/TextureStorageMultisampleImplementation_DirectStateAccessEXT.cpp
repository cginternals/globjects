
#include "TextureStorageMultisampleImplementation_DirectStateAccessEXT.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>


using namespace gl;


namespace globjects
{


TextureStorageMultisampleImplementation_DirectStateAccessEXT::TextureStorageMultisampleImplementation_DirectStateAccessEXT()
{
}

TextureStorageMultisampleImplementation_DirectStateAccessEXT::~TextureStorageMultisampleImplementation_DirectStateAccessEXT()
{
}

void TextureStorageMultisampleImplementation_DirectStateAccessEXT::storage2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSampleLocations) const
{
    gl::glTextureStorage2DMultisampleEXT(texture->id(), texture->target(), samples, internalFormat, width, height, fixedSampleLocations);
}

void TextureStorageMultisampleImplementation_DirectStateAccessEXT::storage3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSampleLocations) const
{
    gl::glTextureStorage3DMultisampleEXT(texture->id(), texture->target(), samples, internalFormat, width, height, depth, fixedSampleLocations);
}


} // namespace globjects
