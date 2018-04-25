
#include "TextureImplementation_DirectStateAccessARB.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include "TextureImplementation_Legacy.h"
#include "TextureImplementation_DirectStateAccessEXT.h"

using namespace gl;


namespace globjects
{


TextureImplementation_DirectStateAccessARB::TextureImplementation_DirectStateAccessARB()
{
}

TextureImplementation_DirectStateAccessARB::~TextureImplementation_DirectStateAccessARB()
{
}

gl::GLuint TextureImplementation_DirectStateAccessARB::create(gl::GLenum target) const
{
    gl::GLuint result = 0;

    gl::glCreateTextures(target, 1, &result);

    return result;
}

void TextureImplementation_DirectStateAccessARB::destroy(gl::GLuint id) const
{
    get(Texture::BindlessImplementation::Legacy)->destroy(id);
}

void TextureImplementation_DirectStateAccessARB::setParameter(const Texture * texture, gl::GLenum name, gl::GLint value) const
{
    gl::glTextureParameteri(texture->id(), name, value);
}

void TextureImplementation_DirectStateAccessARB::setParameter(const Texture * texture, gl::GLenum name, gl::GLfloat value) const
{
    gl::glTextureParameterf(texture->id(), name, value);
}

void TextureImplementation_DirectStateAccessARB::setParameter(const Texture * texture, gl::GLenum name, const glm::vec4 & value) const
{
    gl::glTextureParameterfv(texture->id(), name, glm::value_ptr(value));
}

void TextureImplementation_DirectStateAccessARB::generateMipMap(const Texture *texture) const
{
    gl::glGenerateTextureMipmap(texture->id());
}

gl::GLint TextureImplementation_DirectStateAccessARB::getParameter(const Texture * texture, gl::GLenum name) const
{
    gl::GLint value = 0;

    gl::glGetTextureParameteriv(texture->id(), name, &value);

    return value;
}

gl::GLint TextureImplementation_DirectStateAccessARB::getLevelParameter(const Texture * texture, gl::GLint level, gl::GLenum name) const
{
    gl::GLint value = 0;

    gl::glGetTextureLevelParameteriv(texture->id(), level, name, &value);

    return value;
}

void TextureImplementation_DirectStateAccessARB::image1D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    get(Texture::BindlessImplementation::DirectStateAccessEXT)->image1D(texture, level, internalFormat, width, border, format, type, data);
}

void TextureImplementation_DirectStateAccessARB::compressedImage1D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const
{
    get(Texture::BindlessImplementation::DirectStateAccessEXT)->compressedImage1D(texture, level, internalFormat, width, border, imageSize, data);
}

void TextureImplementation_DirectStateAccessARB::subImage1D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLsizei width, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    gl::glTextureSubImage1D(texture->id(), level, xOffset, width, format, type, data);
}

void TextureImplementation_DirectStateAccessARB::image2D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    get(Texture::BindlessImplementation::DirectStateAccessEXT)->image2D(texture, level, internalFormat, width, height, border, format, type, data);
}

void TextureImplementation_DirectStateAccessARB::compressedImage2D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const
{
    get(Texture::BindlessImplementation::DirectStateAccessEXT)->compressedImage2D(texture, level, internalFormat, width, height, border, imageSize, data);
}

void TextureImplementation_DirectStateAccessARB::subImage2D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLsizei width, gl::GLsizei height, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    gl::glTextureSubImage2D(texture->id(), level, xOffset, yOffset, width, height, format, type, data);
}

void TextureImplementation_DirectStateAccessARB::image3D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    get(Texture::BindlessImplementation::DirectStateAccessEXT)->image3D(texture, level, internalFormat, width, height, depth, border, format, type, data);
}

void TextureImplementation_DirectStateAccessARB::compressedImage3D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const
{
    get(Texture::BindlessImplementation::DirectStateAccessEXT)->compressedImage3D(texture, level, internalFormat, width, height, depth, border, imageSize, data);
}

void TextureImplementation_DirectStateAccessARB::subImage3D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLint zOffset, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    gl::glTextureSubImage3D(texture->id(), level, xOffset, yOffset, zOffset, width, height, depth, format, type, data);
}

void TextureImplementation_DirectStateAccessARB::image2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSamplesLocations) const
{
    TextureImplementation_Legacy::instance()->image2DMultisample(texture, samples, internalFormat, width, height, fixedSamplesLocations);
}

void TextureImplementation_DirectStateAccessARB::image3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSamplesLocations) const
{
    TextureImplementation_Legacy::instance()->image3DMultisample(texture, samples, internalFormat, width, height, depth, fixedSamplesLocations);
}

void TextureImplementation_DirectStateAccessARB::cubeMapImage(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    get(Texture::BindlessImplementation::DirectStateAccessEXT)->cubeMapImage(texture, level, internalFormat, width, height, border, format, type, data);
}

void TextureImplementation_DirectStateAccessARB::texBuffer(const Texture * texture, const gl::GLenum internalFormat, Buffer * buffer) const
{
    gl::glTextureBuffer(texture->id(), internalFormat, buffer ? buffer->id() : 0);
}

void TextureImplementation_DirectStateAccessARB::texBufferRange(const Texture * texture, const gl::GLenum internalFormat, Buffer * buffer, const gl::GLintptr offset, const gl::GLsizeiptr size) const
{
    gl::glTextureBufferRange(texture->id(), internalFormat, buffer ? buffer->id() : 0, offset, size);
}

void TextureImplementation_DirectStateAccessARB::pageCommitment(const Texture * texture, const gl::GLint level, const gl::GLint xOffset, const gl::GLint yOffset, const gl::GLint zOffset, const gl::GLsizei width, const gl::GLsizei height, const gl::GLsizei depth, const gl::GLboolean commit) const
{
    get(Texture::BindlessImplementation::DirectStateAccessEXT)->pageCommitment(texture, level, xOffset, yOffset, zOffset, width, height, depth, commit);
}


} // namespace globjects
