
#include "TextureImplementation_DirectStateAccessEXT.h"

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include "TextureImplementation_Legacy.h"

using namespace gl;


namespace globjects
{


TextureImplementation_DirectStateAccessEXT::TextureImplementation_DirectStateAccessEXT()
{
}

TextureImplementation_DirectStateAccessEXT::~TextureImplementation_DirectStateAccessEXT()
{
}

gl::GLuint TextureImplementation_DirectStateAccessEXT::create(GLenum target) const
{
    return TextureImplementation_Legacy::instance()->create(target);
}

void TextureImplementation_DirectStateAccessEXT::destroy(gl::GLuint id) const
{
    TextureImplementation_Legacy::instance()->destroy(id);
}

void TextureImplementation_DirectStateAccessEXT::setParameter(const Texture * texture, gl::GLenum name, gl::GLint value) const
{
    gl::glTextureParameteriEXT(texture->id(), texture->target(), name, value);
}

void TextureImplementation_DirectStateAccessEXT::setParameter(const Texture * texture, gl::GLenum name, gl::GLfloat value) const
{
    gl::glTextureParameterfEXT(texture->id(), texture->target(), name, value);
}

void TextureImplementation_DirectStateAccessEXT::setParameter(const Texture * texture, gl::GLenum name, const glm::vec4 & value) const
{
    gl::glTextureParameterfvEXT(texture->id(), texture->target(), name, glm::value_ptr(value));
}

void TextureImplementation_DirectStateAccessEXT::generateMipMap(const Texture *texture) const
{
    gl::glGenerateTextureMipmapEXT(texture->id(), texture->target());
}

gl::GLint TextureImplementation_DirectStateAccessEXT::getParameter(const Texture * texture, gl::GLenum name) const
{
    gl::GLint value = 0;

    gl::glGetTextureParameterivEXT(texture->id(), texture->target(), name, &value);

    return value;
}

gl::GLint TextureImplementation_DirectStateAccessEXT::getLevelParameter(const Texture * texture, gl::GLint level, gl::GLenum name) const
{
    gl::GLint value = 0;

    gl::glGetTextureLevelParameterivEXT(texture->id(), texture->target(), level, name, &value);

    return value;
}

void TextureImplementation_DirectStateAccessEXT::image1D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    gl::glTextureImage1DEXT(texture->id(), texture->target(), level, static_cast<gl::GLint>(internalFormat), width, border, format, type, data);
}

void TextureImplementation_DirectStateAccessEXT::compressedImage1D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const
{
    gl::glCompressedTextureImage1DEXT(texture->id(), texture->target(), level, internalFormat, width, border, imageSize, data);
}

void TextureImplementation_DirectStateAccessEXT::subImage1D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLsizei width, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    gl::glTextureSubImage1DEXT(texture->id(), texture->target(), level, xOffset, width, format, type, data);
}

void TextureImplementation_DirectStateAccessEXT::image2D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    gl::glTextureImage2DEXT(texture->id(), texture->target(), level, static_cast<gl::GLint>(internalFormat), width, height, border, format, type, data);
}

void TextureImplementation_DirectStateAccessEXT::compressedImage2D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const
{
    gl::glCompressedTextureImage2DEXT(texture->id(), texture->target(), level, internalFormat, width, height, border, imageSize, data);
}

void TextureImplementation_DirectStateAccessEXT::subImage2D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLsizei width, gl::GLsizei height, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    gl::glTextureSubImage2DEXT(texture->id(), texture->target(), level, xOffset, yOffset, width, height, format, type, data);
}

void TextureImplementation_DirectStateAccessEXT::image3D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    gl::glTextureImage3DEXT(texture->id(), texture->target(), level, static_cast<gl::GLint>(internalFormat), width, height, depth, border, format, type, data);
}

void TextureImplementation_DirectStateAccessEXT::compressedImage3D(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data) const
{
    gl::glCompressedTextureImage3DEXT(texture->id(), texture->target(), level, internalFormat, width, height, depth, border, imageSize, data);
}

void TextureImplementation_DirectStateAccessEXT::subImage3D(const Texture * texture, gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLint zOffset, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    gl::glTextureSubImage3DEXT(texture->id(), texture->target(), level, xOffset, yOffset, zOffset, width, height, depth, format, type, data);
}

void TextureImplementation_DirectStateAccessEXT::image2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSamplesLocations) const
{
    TextureImplementation_Legacy::instance()->image2DMultisample(texture, samples, internalFormat, width, height, fixedSamplesLocations);
}

void TextureImplementation_DirectStateAccessEXT::image3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSamplesLocations) const
{
    TextureImplementation_Legacy::instance()->image3DMultisample(texture, samples, internalFormat, width, height, depth, fixedSamplesLocations);
}

void TextureImplementation_DirectStateAccessEXT::cubeMapImage(const Texture * texture, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data) const
{
    texture->bind();

    for (int i = 0; i < 6; ++i)
    {
        gl::glTextureImage2DEXT(texture->id(), gl::GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, static_cast<gl::GLint>(internalFormat), width, height, border, format, type, data);
    }
}

void TextureImplementation_DirectStateAccessEXT::texBuffer(const Texture * texture, const gl::GLenum internalFormat, Buffer * buffer) const
{
    gl::glTextureBufferEXT(texture->id(), texture->target(), internalFormat, buffer ? buffer->id() : 0);
}

void TextureImplementation_DirectStateAccessEXT::texBufferRange(const Texture * texture, const gl::GLenum internalFormat, Buffer * buffer, const gl::GLintptr offset, const gl::GLsizeiptr size) const
{
    gl::glTextureBufferRangeEXT(texture->id(), texture->target(), internalFormat, buffer ? buffer->id() : 0, offset, size);
}

void TextureImplementation_DirectStateAccessEXT::pageCommitment(const Texture * texture, const gl::GLint level, const gl::GLint xOffset, const gl::GLint yOffset, const gl::GLint zOffset, const gl::GLsizei width, const gl::GLsizei height, const gl::GLsizei depth, const gl::GLboolean commit) const
{
    gl::glTexturePageCommitmentEXT(texture->id(), level, xOffset, yOffset, zOffset, width, height, depth, commit);
}


} // namespace globjects
