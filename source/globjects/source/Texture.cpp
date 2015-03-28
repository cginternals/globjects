#include <globjects/Texture.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <glm/gtc/type_ptr.hpp>

#include <globjects/Buffer.h>
#include <globjects/ObjectVisitor.h>

#include "pixelformat.h"
#include "Resource.h"


using namespace gl;

namespace globjects
{

Texture::Texture()
: Texture(GL_TEXTURE_2D)
{
}

Texture::Texture(const GLenum target)
: Object(new TextureResource)
, m_target(target)
{
}

Texture::Texture(IDResource * resource, const GLenum target)
: Object(resource)
, m_target(target)
{
}

Texture * Texture::fromId(const GLuint id, const GLenum target)
{
    return new Texture(new ExternalResource(id), target);
}


Texture::~Texture()
{
}

Texture * Texture::createDefault()
{
    return createDefault(GL_TEXTURE_2D);
}

Texture * Texture::createDefault(const GLenum target)
{
    Texture* texture = new Texture(target);

    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}

void Texture::bind() const
{
    glBindTexture(m_target, id());
}

void Texture::unbind() const
{
    unbind(m_target);
}

void Texture::unbind(const GLenum target)
{
    glBindTexture(target, 0);
}

void Texture::bindActive(const GLenum texture) const
{
    glActiveTexture(texture);
    glBindTexture(m_target, id());
}

void Texture::unbindActive(const GLenum texture) const
{
    glActiveTexture(texture);
    glBindTexture(m_target, 0);
}

GLenum Texture::target() const
{
    return m_target;
}

void Texture::setParameter(const GLenum name, const GLenum value)
{
    setParameter(name, static_cast<GLint>(value));
}

void Texture::setParameter(const GLenum name, const GLint value)
{
	bind();

    glTexParameteri(m_target, name, value);
}

void Texture::setParameter(const GLenum name, const GLfloat value)
{
	bind();

    glTexParameterf(m_target, name, value);
}

GLint Texture::getParameter(const GLenum pname) const
{
	bind();

	GLint value = 0;

    glGetTexParameteriv(m_target, pname, &value);

	return value;
}

GLint Texture::getLevelParameter(const GLint level, const GLenum pname) const
{
	bind();

	GLint value = 0;

    glGetTexLevelParameteriv(m_target, level, pname, &value);

	return value;
}

void Texture::getImage(const GLint level, const GLenum format, const GLenum type, GLvoid * image) const
{
    bind();

    glGetTexImage(m_target, level, format, type, image);
}

std::vector<unsigned char> Texture::getImage(const GLint level, const GLenum format, const GLenum type) const
{
    GLint width = getLevelParameter(level, GL_TEXTURE_WIDTH);
    GLint height = getLevelParameter(level, GL_TEXTURE_HEIGHT);

    int byteSize = imageSizeInBytes(width, height, format, type);

    std::vector<unsigned char> data(byteSize);
    getImage(level, format, type, data.data());

    return data;
}

void Texture::getCompressedImage(const GLint lod, GLvoid * image) const
{
    bind();

    glGetCompressedTexImage(m_target, lod, image);
}

std::vector<unsigned char> Texture::getCompressedImage(const GLint lod) const
{
    GLint size = getLevelParameter(lod, GL_TEXTURE_COMPRESSED_IMAGE_SIZE);

    std::vector<unsigned char> data(size);
    getCompressedImage(lod, data.data());

    return data;
}

void Texture::image1D(const GLint level, const GLenum internalFormat, const GLsizei width, const GLint border, const GLenum format, const GLenum type, const GLvoid * data)
{
    bind();

    glTexImage1D(m_target, level, static_cast<GLint>(internalFormat), width, border, format, type, data);
}

void Texture::compressedImage1D(const GLint level, const GLenum internalFormat, const GLsizei width, const GLint border, const GLsizei imageSize, const GLvoid * data)
{
    bind();

    glCompressedTexImage1D(m_target, level, internalFormat, width, border, imageSize, data);
}

void Texture::subImage1D(const GLint level, const GLint xOffset, const GLsizei width, const GLenum format, const GLenum type, const GLvoid * data)
{
    bind();

    glTexSubImage1D(m_target, level, xOffset, width, format, type, data);
}

void Texture::image2D(const GLint level, const GLenum internalFormat, const GLsizei width, const GLsizei height, const GLint border, const GLenum format, const GLenum type, const GLvoid* data)
{
	bind();

    glTexImage2D(m_target, level, static_cast<GLint>(internalFormat), width, height, border, format, type, data);
}

void Texture::image2D(const GLint level, const GLenum internalFormat, const glm::ivec2 & size, const GLint border, const GLenum format, const GLenum type, const GLvoid* data)
{
    image2D(level, internalFormat, size.x, size.y, border, format, type, data);
}

void Texture::image2D(const GLenum target, const GLint level, const GLenum internalFormat, const GLsizei width, const GLsizei height, const GLint border, const GLenum format, const GLenum type, const GLvoid* data)
{
    bind();

    glTexImage2D(target, level, static_cast<GLint>(internalFormat), width, height, border, format, type, data);
}

void Texture::image2D(const GLenum target, const GLint level, const GLenum internalFormat, const glm::ivec2 & size, const GLint border, const GLenum format, const GLenum type, const GLvoid* data)
{
    image2D(target, level, internalFormat, size.x, size.y, border, format, type, data);
}

void Texture::compressedImage2D(const GLint level, const GLenum internalFormat, const GLsizei width, const GLsizei height, const GLint border, const GLsizei imageSize, const GLvoid * data)
{
    bind();

    glCompressedTexImage2D(m_target, level, internalFormat, width, height, border, imageSize, data);
}

void Texture::compressedImage2D(const GLint level, const GLenum internalFormat, const glm::ivec2 & size, const GLint border, const GLsizei imageSize, const GLvoid * data)
{
    compressedImage2D(level, internalFormat, size.x, size.y, border, imageSize, data);
}

void Texture::subImage2D(const GLint level, const GLint xOffset, const GLint yOffset, const GLsizei width, const GLsizei height, const GLenum format, const GLenum type, const GLvoid * data)
{
    bind();

    glTexSubImage2D(m_target, level, xOffset, yOffset, width, height, format, type, data);
}

void Texture::subImage2D(const GLint level, const glm::ivec2& offset, const glm::ivec2& size, const GLenum format, const GLenum type, const GLvoid * data)
{
    subImage2D(level, offset.x, offset.y, size.x, size.y, format, type, data);
}

void Texture::image3D(const GLint level, const GLenum internalFormat, const GLsizei width, const GLsizei height, const GLsizei depth, const GLint border, const GLenum format, const GLenum type, const GLvoid* data)
{
    bind();

    glTexImage3D(m_target, level, static_cast<GLint>(internalFormat), width, height, depth, border, format, type, data);
}

void Texture::image3D(const GLint level, const GLenum internalFormat, const glm::ivec3 & size, const GLint border, const GLenum format, const GLenum type, const GLvoid* data)
{
    image3D(level, internalFormat, size.x, size.y, size.z, border, format, type, data);
}

void Texture::compressedImage3D(const GLint level, const GLenum internalFormat, const GLsizei width, const GLsizei height, const GLsizei depth, const GLint border, const GLsizei imageSize, const GLvoid * data)
{
    bind();

    glCompressedTexImage3D(m_target, level, internalFormat, width, height, depth, border, imageSize, data);
}

void Texture::compressedImage3D(GLint level, GLenum internalFormat, const glm::ivec3 & size, GLint border, GLsizei imageSize, const GLvoid * data)
{
    compressedImage3D(level, internalFormat, size.x, size.y, size.z, border, imageSize, data);
}

void Texture::subImage3D(const GLint level, const GLint xOffset, const GLint yOffset, const GLint zOffset, const GLsizei width, const GLsizei height, const GLsizei depth, const GLenum format, const GLenum type, const GLvoid * data)
{
    bind();

    glTexSubImage3D(m_target, level, xOffset, yOffset, zOffset, width, height, depth, format, type, data);
}

void Texture::subImage3D(const GLint level, const glm::ivec3& offset, const glm::ivec3& size, const GLenum format, const GLenum type, const GLvoid * data)
{
    subImage3D(level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, data);
}

void Texture::image2DMultisample(const GLsizei samples, const GLenum internalFormat, const GLsizei width, const GLsizei height, const GLboolean fixedSamplesLocations)
{
    bind();

    glTexImage2DMultisample(m_target, samples, internalFormat, width, height, fixedSamplesLocations);
}

void Texture::image2DMultisample(const GLsizei samples, const GLenum internalFormat, const glm::ivec2 & size, const GLboolean fixedSamplesLocations)
{
    image2DMultisample(samples, internalFormat, size.x, size.y, fixedSamplesLocations);
}

void Texture::image3DMultisample(const GLsizei samples, const GLenum internalFormat, const GLsizei width, const GLsizei height, const GLsizei depth, const GLboolean fixedSamplesLocations)
{
    bind();

    glTexImage3DMultisample(m_target, samples, internalFormat, width, height, depth, fixedSamplesLocations);
}

void Texture::image3DMultisample(const GLsizei samples, const GLenum internalFormat, const glm::ivec3 & size, const GLboolean fixedSamplesLocations)
{
    image3DMultisample(samples, internalFormat, size.x, size.y, size.z, fixedSamplesLocations);
}

void Texture::storage1D(const GLsizei levels, const GLenum internalFormat, const GLsizei width)
{
    bind();

    glTexStorage1D(m_target, levels, internalFormat, width);
}

void Texture::storage2D(const GLsizei levels, const GLenum internalFormat, const GLsizei width, const GLsizei height)
{
	bind();

    glTexStorage2D(m_target, levels, internalFormat, width, height);
}

void Texture::storage2D(const GLsizei levels, const GLenum internalFormat, const glm::ivec2 & size)
{
    storage2D(levels, internalFormat, size.x, size.y);
}

void Texture::storage3D(const GLsizei levels, const GLenum internalFormat, const GLsizei width, const GLsizei height, const GLsizei depth)
{
    bind();

    glTexStorage3D(m_target, levels, internalFormat, width, height, depth);
}

void Texture::storage3D(const GLsizei levels, const GLenum internalFormat, const glm::ivec3 & size)
{
    storage3D(levels, internalFormat, size.x, size.y, size.z);
}

void Texture::textureView(const GLuint originalTexture, const GLenum internalFormat, const GLuint minLevel, const GLuint numLevels, const GLuint minLayer, const GLuint numLayers)
{
    glTextureView(id(), m_target, originalTexture, internalFormat, minLevel, numLevels, minLayer, numLayers);
}

void Texture::texBuffer(const GLenum internalFormat, Buffer * buffer)
{
    bind();

    glTexBuffer(m_target, internalFormat, buffer ? buffer->id() : 0);
}

void Texture::texBuffer(const GLenum activeTexture, const GLenum internalFormat, Buffer * buffer)
{
    bindActive(activeTexture);
    texBuffer(internalFormat, buffer);
}

void Texture::texBufferRange(const GLenum internalFormat, Buffer * buffer, const GLintptr offset, const GLsizeiptr size)
{
    bind();

    glTexBufferRange(m_target, internalFormat, buffer ? buffer->id() : 0, offset, size);
}

void Texture::texBufferRange(const GLenum activeTexture, const GLenum internalFormat, Buffer * buffer, const GLintptr offset, const GLsizeiptr size)
{
    bindActive(activeTexture);
    texBufferRange(internalFormat, buffer, offset, size);
}

void Texture::clearImage(const GLint level, const GLenum format, const GLenum type, const void * data)
{
    glClearTexImage(id(), level, format, type, data);
}

void Texture::clearImage(const GLint level, const GLenum format, const GLenum type, const glm::vec4 & value)
{
    clearImage(level, format, type, glm::value_ptr(value));
}

void Texture::clearImage(const GLint level, const GLenum format, const GLenum type, const glm::ivec4 & value)
{
    clearImage(level, format, type, glm::value_ptr(value));
}

void Texture::clearImage(const GLint level, const GLenum format, const GLenum type, const glm::uvec4 & value)
{
    clearImage(level, format, type, glm::value_ptr(value));
}

void Texture::clearSubImage(const GLint level, const GLint xOffset, const GLint yOffset, const GLint zOffset, const GLsizei width, const GLsizei height, const GLsizei depth, const GLenum format, const GLenum type, const void * data)
{
    glClearTexSubImage(id(), level, xOffset, yOffset, zOffset, width, height, depth, format, type, data);
}

void Texture::clearSubImage(const GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, const GLenum format, const GLenum type, const void * data)
{
    clearSubImage(level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, data);
}

void Texture::clearSubImage(const GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, const GLenum format, const GLenum type, const glm::vec4 & value)
{
    clearSubImage(level, offset, size, format, type, glm::value_ptr(value));
}

void Texture::clearSubImage(const GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, const GLenum format, const GLenum type, const glm::ivec4 & value)
{
    clearSubImage(level, offset, size, format, type, glm::value_ptr(value));
}

void Texture::clearSubImage(const GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, const GLenum format, const GLenum type, const glm::uvec4 & value)
{
    clearSubImage(level, offset, size, format, type, glm::value_ptr(value));
}

void Texture::invalidateImage(GLint level) const
{
    glInvalidateTexImage(id(), level);
}

void Texture::invalidateSubImage(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth)
{
    glInvalidateTexSubImage(id(), level, xoffset, yoffset, zoffset, width, height, depth);
}

void Texture::invalidateSubImage(GLint level, const glm::ivec3& offset, const glm::ivec3 size)
{
    invalidateSubImage(level, offset.x, offset.y, offset.z, size.x, size.y, size.z);
}

void Texture::bindImageTexture(const GLuint unit, const GLint level, const GLboolean layered, const GLint layer, const GLenum access, const GLenum format) const
{
	glBindImageTexture(unit, id(), level, layered, layer, access, format);
}

void Texture::unbindImageTexture(const GLuint unit)
{
    // the concrete parameters (except unit & texture) don't seem to matter, as long as their values are valid
    glBindImageTexture(unit, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
}

void Texture::generateMipmap()
{
	bind();

    glGenerateMipmap(m_target);
}

void Texture::accept(ObjectVisitor& visitor)
{
	visitor.visitTexture(this);
}

TextureHandle Texture::textureHandle() const
{
    return glGetTextureHandleARB(id());
}

bool Texture::isResident() const
{
    return glIsTextureHandleResidentARB(textureHandle()) == GL_TRUE;
}

TextureHandle Texture::makeResident() const
{
    TextureHandle handle = textureHandle();

    glMakeTextureHandleResidentARB(handle);

	return handle;
}

void Texture::makeNonResident() const
{
    glMakeTextureHandleNonResidentARB(textureHandle());
}

void Texture::pageCommitment(const GLint level, const GLint xOffset, const GLint yOffset, const GLint zOffset, const GLsizei width, const GLsizei height, const GLsizei depth, const GLboolean commit) const
{
    bind();

    glTexPageCommitmentARB(m_target, level, xOffset, yOffset, zOffset, width, height, depth, commit);
}

void Texture::pageCommitment(const GLint level, const glm::ivec3& offset, const glm::ivec3& size, const GLboolean commit) const
{
    pageCommitment(level, offset.x, offset.y, offset.z, size.x, size.y, size.z, commit);
}

GLenum Texture::objectType() const
{
    return GL_TEXTURE;
}

} // namespace globjects
