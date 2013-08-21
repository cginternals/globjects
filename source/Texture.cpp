
#include <glow/Texture.h>

#include <glow/Error.h>

namespace glow
{

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
		CheckGLError();
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
	CheckGLError();
}

void Texture::setParameter(GLenum name, GLfloat value)
{
	bind();
	
	glTexParameterf(_target, name, value);
	CheckGLError();
}

void Texture::bind()
{
	glBindTexture(_target, m_id);
	CheckGLError();
}

void Texture::unbind()
{
	glBindTexture(_target, 0);
	CheckGLError();
}

void Texture::image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
{
	bind();
	
	glTexImage2D(_target, level, internalFormat, width, height, border, format, type, data);
	CheckGLError();
}

void Texture::storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
	bind();
	
	glTexStorage2D(_target, levels, internalFormat, width, height);
	CheckGLError();
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
	
	glGenerateMipmap(_target);
	CheckGLError();
}

GLuint Texture::genTexture()
{
	GLuint id = 0;
	
	glGenTextures(1, &id);
	CheckGLError();
	
	return id;
}

Texture::Handle Texture::textureHandle() const
{
	Texture::Handle result(glGetTextureHandleNV(m_id));
	CheckGLError();
	return result;
}

GLboolean Texture::isResident() const
{
	bool result = glIsTextureHandleResidentNV(textureHandle()) ? true : false;
	CheckGLError();

	return result;
}

Texture::Handle Texture::makeResident()
{
	Handle handle = textureHandle();

	glMakeTextureHandleResidentNV(handle);
	CheckGLError();
	
	return handle;
}

void Texture::makeNonResident()
{
	glMakeTextureHandleNonResidentNV(textureHandle());
	CheckGLError();
}

} // namespace glow
