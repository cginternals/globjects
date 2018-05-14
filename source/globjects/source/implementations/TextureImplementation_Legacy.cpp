
#include "TextureImplementation_Legacy.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>


namespace globjects
{


TextureImplementation_Legacy::TextureImplementation_Legacy()
{
}

TextureImplementation_Legacy::~TextureImplementation_Legacy()
{
}

gl::GLuint TextureImplementation_Legacy::create(gl::GLenum target) const
{
    gl::GLuint result = 0;

    gl::glGenTextures(1, &result);
    gl::glBindTexture(target, result);

    return result;
}

void TextureImplementation_Legacy::destroy(gl::GLuint id) const
{
    gl::glDeleteTextures(1, &id);
}


void TextureImplementation_Legacy::bindActive(const Texture * texture, gl::GLuint unit) const
{
    gl::glActiveTexture(gl::GL_TEXTURE0 + unit);
    gl::glBindTexture(texture->target(), texture->id());
}


void TextureImplementation_Legacy::unbindActive(const Texture * texture, gl::GLuint unit) const
{
    gl::glActiveTexture(gl::GL_TEXTURE0 + unit);
    gl::glBindTexture(texture->target(), 0);
}


void TextureImplementation_Legacy::setParameter(const Texture * texture, gl::GLenum name, gl::GLint value) const
{
    texture->bind();

    gl::glTexParameteri(texture->target(), name, value);
}

void TextureImplementation_Legacy::setParameter(const Texture * texture, gl::GLenum name, gl::GLfloat value) const
{
    texture->bind();

    gl::glTexParameterf(texture->target(), name, value);
}

void TextureImplementation_Legacy::setParameter(const Texture * texture, gl::GLenum name, const glm::vec4 & value) const
{
    texture->bind();

    gl::glTexParameterfv(texture->target(), name, glm::value_ptr(value));
}

void TextureImplementation_Legacy::generateMipMap(const Texture *texture) const
{
    texture->bind();

    gl::glGenerateMipmap(texture->target());
}

gl::GLint TextureImplementation_Legacy::getParameter(const Texture * texture, gl::GLenum name) const
{
    texture->bind();

    gl::GLint value = 0;

    gl::glGetTexParameteriv(texture->target(), name, &value);

    return value;
}

gl::GLint TextureImplementation_Legacy::getLevelParameter(const Texture * texture, gl::GLint level, gl::GLenum name) const
{
    texture->bind();

    gl::GLint value = 0;

    gl::glGetTexLevelParameteriv(texture->target(), level, name, &value);

    return value;
}

void TextureImplementation_Legacy::image1D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    texture->bind();

    gl::glTexImage1D(texture->target(), level, static_cast<gl::GLint>(internalFormat), width, border, format, type, data);
}

void TextureImplementation_Legacy::compressedImage1D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const
{
    texture->bind();

    gl::glCompressedTexImage1D(texture->target(), level, internalFormat, width, border, imageSize, data);
}

void TextureImplementation_Legacy::subImage1D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLsizei width, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    texture->bind();

    gl::glTexSubImage1D(texture->target(), level, xOffset, width, format, type, data);
}

void TextureImplementation_Legacy::image2D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    texture->bind();

    gl::glTexImage2D(texture->target(), level, static_cast<gl::GLint>(internalFormat), width, height, border, format, type, data);
}

void TextureImplementation_Legacy::compressedImage2D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const
{
    texture->bind();

    gl::glCompressedTexImage2D(texture->target(), level, internalFormat, width, height, border, imageSize, data);
}

void TextureImplementation_Legacy::subImage2D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLsizei width, gl::GLsizei height, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    texture->bind();

    gl::glTexSubImage2D(texture->target(), level, xOffset, yOffset, width, height, format, type, data);
}

void TextureImplementation_Legacy::image3D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    texture->bind();

    gl::glTexImage3D(texture->target(), level, static_cast<gl::GLint>(internalFormat), width, height, depth, border, format, type, data);
}

void TextureImplementation_Legacy::compressedImage3D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const
{
    texture->bind();

    gl::glCompressedTexImage3D(texture->target(), level, internalFormat, width, height, depth, border, imageSize, data);
}

void TextureImplementation_Legacy::subImage3D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLint zOffset, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    texture->bind();

    gl::glTexSubImage3D(texture->target(), level, xOffset, yOffset, zOffset, width, height, depth, format, type, data);
}

void TextureImplementation_Legacy::image2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSamplesLocations) const
{
    texture->bind();

    gl::glTexImage2DMultisample(texture->target(), samples, internalFormat, width, height, fixedSamplesLocations);
}

void TextureImplementation_Legacy::image3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSamplesLocations) const
{
    texture->bind();

    gl::glTexImage3DMultisample(texture->target(), samples, internalFormat, width, height, depth, fixedSamplesLocations);
}

void TextureImplementation_Legacy::cubeMapImage(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    texture->bind();

    for (int i = 0; i < 6; ++i)
    {
        gl::glTexImage2D(gl::GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, static_cast<gl::GLint>(internalFormat), width, height, border, format, type, data);
    }
}

void TextureImplementation_Legacy::texBuffer(const Texture * texture, const gl::GLenum internalFormat, Buffer * buffer) const
{
    texture->bind();

    gl::glTexBuffer(texture->target(), internalFormat, buffer ? buffer->id() : 0);
}

void TextureImplementation_Legacy::texBufferRange(const Texture * texture, const gl::GLenum internalFormat, Buffer * buffer, const gl::GLintptr offset, const gl::GLsizeiptr size) const
{
    texture->bind();

    gl::glTexBufferRange(texture->target(), internalFormat, buffer ? buffer->id() : 0, offset, size);
}

void TextureImplementation_Legacy::pageCommitment(const Texture * texture, const gl::GLint level, const gl::GLint xOffset, const gl::GLint yOffset, const gl::GLint zOffset, const gl::GLsizei width, const gl::GLsizei height, const gl::GLsizei depth, const gl::GLboolean commit) const
{
    texture->bind();

    gl::glTexPageCommitmentARB(texture->target(), level, xOffset, yOffset, zOffset, width, height, depth, commit);
}


} // namespace globjects
