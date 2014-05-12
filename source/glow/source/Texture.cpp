#include <glow/Texture.h>

#include <algorithm>

#include <glbinding/constants.h>
#include <glbinding/functions.h>

#include <glm/gtc/type_ptr.hpp>

#include <glow/Error.h>
#include <glow/Buffer.h>
#include <glow/ObjectVisitor.h>

#include "pixelformat.h"

namespace glow
{

Texture::Texture(gl::GLenum  target)
: Object(genTexture())
, m_target(target)
{
}

Texture::Texture(gl::GLuint id, gl::GLenum  target, bool takeOwnership)
: Object(id, takeOwnership)
, m_target(target)
{
}

Texture * Texture::fromId(gl::GLuint id, gl::GLenum  target, bool takeOwnership)
{
    return new Texture(id, target, takeOwnership);
}


Texture::~Texture()
{
	if (ownsGLObject())
	{
		gl::DeleteTextures(1, &m_id);
	}
}

Texture * Texture::createDefault(gl::GLenum target)
{
    Texture* tex = new Texture(target);

    tex->setParameter(gl::TEXTURE_MIN_FILTER, static_cast<gl::GLint>(gl::LINEAR));
    tex->setParameter(gl::TEXTURE_MAG_FILTER, static_cast<gl::GLint>(gl::LINEAR));
    tex->setParameter(gl::TEXTURE_WRAP_S, static_cast<gl::GLint>(gl::CLAMP_TO_EDGE));
    tex->setParameter(gl::TEXTURE_WRAP_T, static_cast<gl::GLint>(gl::CLAMP_TO_EDGE));
    tex->setParameter(gl::TEXTURE_WRAP_R, static_cast<gl::GLint>(gl::CLAMP_TO_EDGE));

    return tex;
}

gl::GLuint Texture::genTexture()
{
    gl::GLuint id = 0;

    gl::GenTextures(1, &id);

    return id;
}

void Texture::bind() const
{
    gl::BindTexture(m_target, m_id);
}

void Texture::unbind() const
{
    unbind(m_target);
}

void Texture::unbind(const gl::GLenum target)
{
    gl::BindTexture(target, 0);
}

void Texture::bindActive(const gl::GLenum texture) const
{
    gl::ActiveTexture(texture);
    gl::BindTexture(m_target, m_id);
}

void Texture::unbindActive(const gl::GLenum texture) const
{
    gl::ActiveTexture(texture);
    gl::BindTexture(m_target, 0);
}

gl::GLenum Texture::target() const
{
    return m_target;
}

void Texture::setParameter(gl::GLenum name, gl::GLint value)
{
	bind();

    gl::TexParameteri(m_target, name, value);
}

void Texture::setParameter(gl::GLenum name, gl::GLfloat value)
{
	bind();

    gl::TexParameterf(m_target, name, value);
}

gl::GLint Texture::getParameter(gl::GLenum pname) const
{
	bind();

	gl::GLint value = 0;

    gl::GetTexParameteriv(m_target, pname, &value);

	return value;
}

gl::GLint Texture::getLevelParameter(gl::GLint level, gl::GLenum pname) const
{
	bind();

	gl::GLint value = 0;

    gl::GetTexLevelParameteriv(m_target, level, pname, &value);

	return value;
}

void Texture::getImage(gl::GLint level, gl::GLenum format, gl::GLenum type, gl::GLvoid * image) const
{
    bind();

    gl::GetTexImage(m_target, level, format, type, image);
}

std::vector<unsigned char> Texture::getImage(gl::GLint level, gl::GLenum format, gl::GLenum type) const
{
    gl::GLint width = getLevelParameter(level, gl::TEXTURE_WIDTH);
    gl::GLint height = getLevelParameter(level, gl::TEXTURE_HEIGHT);

    int byteSize = imageSizeInBytes(width, height, format, type);

    std::vector<unsigned char> data(byteSize);
    getImage(level, format, type, data.data());

    return data;
}

void Texture::getCompressedImage(gl::GLint lod, gl::GLvoid * image) const
{
    bind();

    gl::GetCompressedTexImage(m_target, lod, image);
}

std::vector<unsigned char> Texture::getCompressedImage(gl::GLint lod) const
{
    gl::GLint size = getLevelParameter(lod, gl::TEXTURE_COMPRESSED_IMAGE_SIZE);

    std::vector<unsigned char> data(size);
    getCompressedImage(lod, data.data());

    return data;
}

void Texture::image1D(gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid* data)
{
    bind();

    gl::TexImage1D(m_target, level, internalFormat, width, border, format, type, data);
}

void Texture::compressedImage1D(gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data)
{
    bind();

    gl::CompressedTexImage1D(m_target, level, internalFormat, width, border, imageSize, data);
}

void Texture::subImage1D(gl::GLint level, gl::GLint xOffset, gl::GLsizei width, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data)
{
    bind();

    gl::TexSubImage1D(m_target, level, xOffset, width, format, type, data);
}

void Texture::image2D(gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid* data)
{
	bind();

    gl::TexImage2D(m_target, level, internalFormat, width, height, border, format, type, data);
}

void Texture::image2D(gl::GLint level, gl::GLenum internalFormat, const glm::ivec2 & size, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid* data)
{
    image2D(level, internalFormat, size.x, size.y, border, format, type, data);
}

void Texture::image2D(gl::GLenum target, gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid* data)
{
    bind();

    gl::TexImage2D(target, level, internalFormat, width, height, border, format, type, data);
}

void Texture::image2D(gl::GLenum target, gl::GLint level, gl::GLenum internalFormat, const glm::ivec2 & size, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid* data)
{
    image2D(target, level, internalFormat, size.x, size.y, border, format, type, data);
}

void Texture::compressedImage2D(gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data)
{
    bind();

    gl::CompressedTexImage2D(m_target, level, internalFormat, width, height, border, imageSize, data);
}

void Texture::compressedImage2D(gl::GLint level, gl::GLenum internalFormat, const glm::ivec2 & size, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data)
{
    compressedImage2D(level, internalFormat, size.x, size.y, border, imageSize, data);
}

void Texture::subImage2D(gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLsizei width, gl::GLsizei height, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data)
{
    bind();

    gl::TexSubImage2D(m_target, level, xOffset, yOffset, width, height, format, type, data);
}

void Texture::subImage2D(gl::GLint level, const glm::ivec2& offset, const glm::ivec2& size, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data)
{
    subImage2D(level, offset.x, offset.y, size.x, size.y, format, type, data);
}

void Texture::image3D(gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid* data)
{
    bind();

    gl::TexImage3D(m_target, level, internalFormat, width, height, depth, border, format, type, data);
}

void Texture::image3D(gl::GLint level, gl::GLenum internalFormat, const glm::ivec3 & size, gl::GLint border, gl::GLenum format, gl::GLenum type, const gl::GLvoid* data)
{
    image3D(level, internalFormat, size.x, size.y, size.z, border, format, type, data);
}

void Texture::compressedImage3D(gl::GLint level, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data)
{
    bind();

    gl::CompressedTexImage3D(m_target, level, internalFormat, width, height, depth, border, imageSize, data);
}

void Texture::compressedImage3D(gl::GLint level, gl::GLenum internalFormat, const glm::ivec3 & size, gl::GLint border, gl::GLsizei imageSize, const gl::GLvoid * data)
{
    compressedImage3D(level, internalFormat, size.x, size.y, size.z, border, imageSize, data);
}

void Texture::subImage3D(gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLint zOffset, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data)
{
    bind();

    gl::TexSubImage3D(m_target, level, xOffset, yOffset, zOffset, width, height, depth, format, type, data);
}

void Texture::subImage3D(gl::GLint level, const glm::ivec3& offset, const glm::ivec3& size, gl::GLenum format, gl::GLenum type, const gl::GLvoid * data)
{
    subImage3D(level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, data);
}

void Texture::image2DMultisample(gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSamplesLocations)
{
    bind();

    gl::TexImage2DMultisample(m_target, samples, internalFormat, width, height, fixedSamplesLocations);
}

void Texture::image2DMultisample(gl::GLsizei samples, gl::GLenum internalFormat, const glm::ivec2 & size, gl::GLboolean fixedSamplesLocations)
{
    image2DMultisample(samples, internalFormat, size.x, size.y, fixedSamplesLocations);
}

void Texture::image3DMultisample(gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSamplesLocations)
{
    bind();

    gl::TexImage3DMultisample(m_target, samples, internalFormat, width, height, depth, fixedSamplesLocations);
}

void Texture::image3DMultisample(gl::GLsizei samples, gl::GLenum internalFormat, const glm::ivec3 & size, gl::GLboolean fixedSamplesLocations)
{
    image3DMultisample(samples, internalFormat, size.x, size.y, size.z, fixedSamplesLocations);
}

void Texture::storage1D(gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width)
{
    bind();

    gl::TexStorage1D(m_target, levels, internalFormat, width);
}

void Texture::storage2D(gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height)
{
	bind();

    gl::TexStorage2D(m_target, levels, internalFormat, width, height);
}

void Texture::storage2D(gl::GLsizei levels, gl::GLenum internalFormat, const glm::ivec2 & size)
{
    storage2D(levels, internalFormat, size.x, size.y);
}

void Texture::storage3D(gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth)
{
    bind();

    gl::TexStorage3D(m_target, levels, internalFormat, width, height, depth);
}

void Texture::storage3D(gl::GLsizei levels, gl::GLenum internalFormat, const glm::ivec3 & size)
{
    storage3D(levels, internalFormat, size.x, size.y, size.z);
}

void Texture::textureView(gl::GLuint originalTexture, gl::GLenum internalFormat, gl::GLuint minLevel, gl::GLuint numLevels, gl::GLuint minLayer, gl::GLuint numLayers)
{
    gl::TextureView(m_id, m_target, originalTexture, internalFormat, minLevel, numLevels, minLayer, numLayers);
}

void Texture::texBuffer(gl::GLenum internalFormat, Buffer * buffer)
{
    bind();

    gl::TexBuffer(m_target, internalFormat, buffer ? buffer->id() : 0);
}

void Texture::texBuffer(gl::GLenum activeTexture, gl::GLenum internalFormat, Buffer * buffer)
{
    bindActive(activeTexture);
    texBuffer(internalFormat, buffer);
}

void Texture::texBufferRange(gl::GLenum internalFormat, Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size)
{
    bind();

    gl::TexBufferRange(m_target, internalFormat, buffer ? buffer->id() : 0, offset, size);
}

void Texture::texBufferRange(gl::GLenum activeTexture, gl::GLenum internalFormat, Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size)
{
    bindActive(activeTexture);
    texBufferRange(internalFormat, buffer, offset, size);
}

void Texture::clearImage(gl::GLint level, gl::GLenum format, gl::GLenum type, const void * data)
{
    gl::ClearTexImage(m_id, level, format, type, data);
}

void Texture::clearImage(gl::GLint level, gl::GLenum format, gl::GLenum type, const glm::vec4 & value)
{
    clearImage(level, format, type, glm::value_ptr(value));
}

void Texture::clearImage(gl::GLint level, gl::GLenum format, gl::GLenum type, const glm::ivec4 & value)
{
    clearImage(level, format, type, glm::value_ptr(value));
}

void Texture::clearImage(gl::GLint level, gl::GLenum format, gl::GLenum type, const glm::uvec4 & value)
{
    clearImage(level, format, type, glm::value_ptr(value));
}

void Texture::clearSubImage(gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLint zOffset, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLenum format, gl::GLenum type, const void * data)
{
    gl::ClearTexSubImage(m_id, level, xOffset, yOffset, zOffset, width, height, depth, format, type, data);
}

void Texture::clearSubImage(gl::GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, gl::GLenum format, gl::GLenum type, const void * data)
{
    clearSubImage(level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, data);
}

void Texture::clearSubImage(gl::GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, gl::GLenum format, gl::GLenum type, const glm::vec4 & value)
{
    clearSubImage(level, offset, size, format, type, glm::value_ptr(value));
}

void Texture::clearSubImage(gl::GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, gl::GLenum format, gl::GLenum type, const glm::ivec4 & value)
{
    clearSubImage(level, offset, size, format, type, glm::value_ptr(value));
}

void Texture::clearSubImage(gl::GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, gl::GLenum format, gl::GLenum type, const glm::uvec4 & value)
{
    clearSubImage(level, offset, size, format, type, glm::value_ptr(value));
}

void Texture::bindImageTexture(gl::GLuint unit, gl::GLint level, gl::GLboolean layered, gl::GLint layer, gl::GLenum access, gl::GLenum format) const
{
	bind();

	gl::BindImageTexture(unit, m_id, level, layered, layer, access, format);
}

void Texture::generateMipmap()
{
	bind();

    gl::GenerateMipmap(m_target);
}

void Texture::accept(ObjectVisitor& visitor)
{
	visitor.visitTexture(this);
}

TextureHandle Texture::textureHandle() const
{
    return gl::GetTextureHandleARB(m_id);
}

gl::GLboolean Texture::isResident() const
{
    return gl::IsTextureHandleResidentARB(textureHandle()) == gl::TRUE_;
}

TextureHandle Texture::makeResident() const
{
    TextureHandle handle = textureHandle();

    gl::MakeTextureHandleResidentARB(handle);

	return handle;
}

void Texture::makeNonResident() const
{
    gl::MakeTextureHandleNonResidentARB(textureHandle());
}

void Texture::pageCommitment(gl::GLint level, gl::GLint xOffset, gl::GLint yOffset, gl::GLint zOffset, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean commit) const
{
    bind();

    gl::TexPageCommitmentARB(m_target, level, xOffset, yOffset, zOffset, width, height, depth, commit);
}

void Texture::pageCommitment(gl::GLint level, const glm::ivec3& offset, const glm::ivec3& size, gl::GLboolean commit) const
{
    pageCommitment(level, offset.x, offset.y, offset.z, size.x, size.y, size.z, commit);
}

} // namespace glow
