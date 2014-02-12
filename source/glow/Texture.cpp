#include <glow/Texture.h>

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>

#include <glow/Error.h>
#include <glow/Buffer.h>
#include <glow/ObjectVisitor.h>

namespace glow
{

Texture::Texture(GLenum  target)
: Object(genTexture())
, m_target(target)
{
}

Texture::Texture(GLuint id, GLenum  target, bool ownsGLObject)
: Object(id, ownsGLObject)
, m_target(target)
{
}

Texture::~Texture()
{
	if (ownsGLObject())
	{
		glDeleteTextures(1, &m_id);
		CheckGLError();
	}
}

GLuint Texture::genTexture()
{
    GLuint id = 0;

    glGenTextures(1, &id);
    CheckGLError();

    return id;
}

void Texture::bind() const
{
    glBindTexture(m_target, m_id);
	CheckGLError();
}

void Texture::unbind() const
{
    glBindTexture(m_target, 0);
	CheckGLError();
}

void Texture::unbind(const GLenum target)
{
    glBindTexture(target, 0);
    CheckGLError();
}

void Texture::bindActive(const GLenum texture) const
{
    glActiveTexture(texture);
    glBindTexture(m_target, m_id);
    CheckGLError();
}

void Texture::unbindActive(const GLenum texture) const
{
    glActiveTexture(texture);
    glBindTexture(m_target, 0);
    CheckGLError();
}

GLenum Texture::target() const
{
    return m_target;
}

void Texture::setParameter(GLenum name, GLint value)
{
	bind();

    glTexParameteri(m_target, name, value);
	CheckGLError();
}

void Texture::setParameter(GLenum name, GLfloat value)
{
	bind();

    glTexParameterf(m_target, name, value);
	CheckGLError();
}

GLint Texture::getParameter(GLenum pname)
{
	bind();

	GLint value = 0;

    glGetTexParameteriv(m_target, pname, &value);
	CheckGLError();

	return value;
}

GLint Texture::getLevelParameter(GLint level, GLenum pname)
{
	bind();

	GLint value = 0;

    glGetTexLevelParameteriv(m_target, level, pname, &value);
	CheckGLError();

	return value;
}

void Texture::image1D(GLint level, GLenum internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
    bind();

    glTexImage1D(m_target, level, internalFormat, width, border, format, type, data);
    CheckGLError();
}

void Texture::compressedImage1D(GLint level, GLenum internalFormat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid * data)
{
    bind();

    glCompressedTexImage1D(m_target, level, internalFormat, width, border, imageSize, data);
    CheckGLError();
}

void Texture::image2D(GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
	bind();

    glTexImage2D(m_target, level, internalFormat, width, height, border, format, type, data);
	CheckGLError();
}

void Texture::image2D(GLint level, GLenum internalFormat, const glm::ivec2 & size, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
    image2D(level, internalFormat, size.x, size.y, border, format, type, data);
}

void Texture::image2D(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
    bind();

    glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
    CheckGLError();
}

void Texture::image2D(GLenum target, GLint level, GLenum internalFormat, const glm::ivec2 & size, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
    image2D(target, level, internalFormat, size.x, size.y, border, format, type, data);
}

void Texture::compressedImage2D(GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data)
{
    bind();

    glCompressedTexImage2D(m_target, level, internalFormat, width, height, border, imageSize, data);
    CheckGLError();
}

void Texture::compressedImage2D(GLint level, GLenum internalFormat, const glm::ivec2 & size, GLint border, GLsizei imageSize, const GLvoid * data)
{
    compressedImage2D(level, internalFormat, size.x, size.y, border, imageSize, data);
}

void Texture::image3D(GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
    bind();

    glTexImage3D(m_target, level, internalFormat, width, height, depth, border, format, type, data);
    CheckGLError();
}

void Texture::image3D(GLint level, GLenum internalFormat, const glm::ivec3 & size, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
    image3D(level, internalFormat, size.x, size.y, size.z, border, format, type, data);
}

void Texture::compressedImage3D(GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid * data)
{
    bind();

    glCompressedTexImage3D(m_target, level, internalFormat, width, height, depth, border, imageSize, data);
    CheckGLError();
}

void Texture::compressedImage3D(GLint level, GLenum internalFormat, const glm::ivec3 & size, GLint border, GLsizei imageSize, const GLvoid * data)
{
    compressedImage3D(level, internalFormat, size.x, size.y, size.z, border, imageSize, data);
}

void Texture::image2DMultisample(GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSamplesLocations)
{
    bind();

    glTexImage2DMultisample(m_target, samples, internalFormat, width, height, fixedSamplesLocations);
    CheckGLError();
}

void Texture::image2DMultisample(GLsizei samples, GLenum internalFormat, const glm::ivec2 & size, GLboolean fixedSamplesLocations)
{
    image2DMultisample(samples, internalFormat, size.x, size.y, fixedSamplesLocations);
}

void Texture::image3DMultisample(GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSamplesLocations)
{
    bind();

    glTexImage3DMultisample(m_target, samples, internalFormat, width, height, depth, fixedSamplesLocations);
    CheckGLError();
}

void Texture::image3DMultisample(GLsizei samples, GLenum internalFormat, const glm::ivec3 & size, GLboolean fixedSamplesLocations)
{
    image3DMultisample(samples, internalFormat, size.x, size.y, size.z, fixedSamplesLocations);
}

void Texture::storage1D(GLsizei levels, GLenum internalFormat, GLsizei width)
{
    bind();

    glTexStorage1D(m_target, levels, internalFormat, width);
    CheckGLError();
}

void Texture::storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
	bind();

    glTexStorage2D(m_target, levels, internalFormat, width, height);
	CheckGLError();
}

void Texture::storage2D(GLsizei levels, GLenum internalFormat, const glm::ivec2 & size)
{
    storage2D(levels, internalFormat, size.x, size.y);
}

void Texture::storage3D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth)
{
    bind();

    glTexStorage3D(m_target, levels, internalFormat, width, height, depth);
    CheckGLError();
}

void Texture::storage3D(GLsizei levels, GLenum internalFormat, const glm::ivec3 & size)
{
    storage3D(levels, internalFormat, size.x, size.y, size.z);
}

void Texture::textureView(GLuint originalTexture, GLenum internalFormat, GLuint minLevel, GLuint numLevels, GLuint minLayer, GLuint numLayers)
{
    glTextureView(m_id, m_target, originalTexture, internalFormat, minLevel, numLevels, minLayer, numLayers);
    CheckGLError();
}

void Texture::texBuffer(GLenum internalFormat, Buffer * buffer)
{
    bind();

    glTexBuffer(m_target, internalFormat, buffer ? buffer->id() : 0);
    CheckGLError();
}

void Texture::texBuffer(GLenum activeTexture, GLenum internalFormat, Buffer * buffer)
{
    bindActive(activeTexture);
    texBuffer(internalFormat, buffer);
}

void Texture::clearImage(GLint level, GLenum format, GLenum type, const void * data)
{
    glClearTexImage(m_id, level, format, type, data);
    CheckGLError();
}

void Texture::clearImage(GLint level, GLenum format, GLenum type, const glm::vec4 & value)
{
    clearImage(level, format, type, glm::value_ptr(value));
}

void Texture::clearImage(GLint level, GLenum format, GLenum type, const glm::ivec4 & value)
{
    clearImage(level, format, type, glm::value_ptr(value));
}

void Texture::clearImage(GLint level, GLenum format, GLenum type, const glm::uvec4 & value)
{
    clearImage(level, format, type, glm::value_ptr(value));
}

void Texture::clearSubImage(GLint level, GLint xOffset, GLint yOffset, GLint zOffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * data)
{
    glClearTexSubImage(m_id, level, xOffset, yOffset, zOffset, width, height, depth, format, type, data);
    CheckGLError();
}

void Texture::clearSubImage(GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, GLenum format, GLenum type, const void * data)
{
    clearSubImage(level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, data);
}

void Texture::clearSubImage(GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, GLenum format, GLenum type, const glm::vec4 & value)
{
    clearSubImage(level, offset, size, format, type, glm::value_ptr(value));
}

void Texture::clearSubImage(GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, GLenum format, GLenum type, const glm::ivec4 & value)
{
    clearSubImage(level, offset, size, format, type, glm::value_ptr(value));
}

void Texture::clearSubImage(GLint level, const glm::ivec3 & offset, const glm::ivec3 & size, GLenum format, GLenum type, const glm::uvec4 & value)
{
    clearSubImage(level, offset, size, format, type, glm::value_ptr(value));
}

void Texture::bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
	bind();

	glBindImageTexture(unit, m_id, level, layered, layer, access, format);
	CheckGLError();
}

void Texture::generateMipmap()
{
	bind();

    glGenerateMipmap(m_target);
	CheckGLError();
}

void Texture::accept(ObjectVisitor& visitor)
{
	visitor.visitTexture(this);
}

TextureHandle Texture::textureHandle() const
{
    TextureHandle result = glGetTextureHandleARB(m_id);
	CheckGLError();
	return result;
}

GLboolean Texture::isResident() const
{
    bool result = glIsTextureHandleResidentARB(textureHandle()) == GL_TRUE;
	CheckGLError();

	return result;
}

TextureHandle Texture::makeResident()
{
    TextureHandle handle = textureHandle();

    glMakeTextureHandleResidentARB(handle);
	CheckGLError();

	return handle;
}

void Texture::makeNonResident()
{
    glMakeTextureHandleNonResidentARB(textureHandle());
	CheckGLError();
}

} // namespace glow
