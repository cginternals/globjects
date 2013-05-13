#include <glow/Texture.h>

using namespace glow;

Texture::Texture(GLenum  target)
: _target(target)
{
	glGenTextures(1, &_id);
}

Texture::~Texture()
{
	if (_id) glDeleteTextures(1, &_id);
}

void Texture::setParameter(GLenum name, GLint value)
{
	bind();
	glTexParameteri(_target, name, value);
	unbind();
}

void Texture::setParameter(GLenum name, GLfloat value)
{
	bind();
	glTexParameterf(_target, name, value);
	unbind();
}

void Texture::bind()
{
	glBindTexture(_target, _id);
}

void Texture::unbind()
{
	glBindTexture(_target, 0);
}

void Texture::image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
	bind();
	glTexImage2D(_target, level, internalFormat, width, height, border, format, type, data);
	unbind();
}

void Texture::storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
	bind();
	glTexStorage2D(_target, levels, internalFormat, width, height);
	unbind();
}

void Texture::bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
	bind();
	glBindImageTexture(unit, _id, level, layered, layer, access, format);
	unbind();
}
