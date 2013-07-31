
#include <glow/Texture.h>

using namespace glow;

Texture::Texture(GLenum  target)
: Object(genTexture())
, _target(target)
{
}

Texture::Texture(GLuint id, GLenum  target, bool ownsGLObject)
: Object(id, ownsGLObject)
, _target(target)
{
}

Texture::~Texture()
{
	if (ownsGLObject())
    {
        glDeleteTextures(1, &m_id);
    }
}

GLenum Texture::target() const
{
	return _target;
}

void Texture::setParameter(GLenum name, GLint value)
{
	bind();
	glTexParameteri(_target, name, value);
}

void Texture::setParameter(GLenum name, GLfloat value)
{
	bind();
	glTexParameterf(_target, name, value);
}

void Texture::bind()
{
	glBindTexture(_target, m_id);
}

void Texture::unbind()
{
	glBindTexture(_target, 0);
}

void Texture::image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
	bind();
	glTexImage2D(_target, level, internalFormat, width, height, border, format, type, data);
}

void Texture::storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
	bind();
	glTexStorage2D(_target, levels, internalFormat, width, height);
}

void Texture::bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
	bind();
	glBindImageTexture(unit, m_id, level, layered, layer, access, format);
}

void Texture::generateMipmap()
{
	bind();
	glGenerateMipmap(_target);
}

GLuint Texture::genTexture()
{
	GLuint id = 0;
	glGenTextures(1, &id);
	return id;
}

#ifdef GL_NV_bindless_texture

Texture::Handle Texture::textureHandle() const
{
	return glGetTextureHandleNV(m_id);
}

bool Texture::isResident() const
{
	return glIsTextureHandleResidentNV(textureHandle());
}

Texture::Handle Texture::makeResident()
{
	Handle handle = textureHandle();

	glMakeTextureHandleResidentNV(handle);
	
	return handle;
}

void Texture::makeNonResident()
{
	glMakeTextureHandleNonResidentNV(textureHandle());
}

#endif
