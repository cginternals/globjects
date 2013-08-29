
#include <algorithm>

#include <glow/Error.h>
#include <glow/logging.h>
#include <glow/Texture.h>
#include <glow/ObjectVisitor.h>

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

GLint Texture::getParameter(GLenum pname)
{
	bind();

	GLint value = 0;

	glGetTexParameteriv(_target, pname, &value);
	CheckGLError();

	return value;
}

GLint Texture::getLevelParameter(GLint level, GLenum pname)
{
	bind();

	GLint value = 0;

	glGetTexLevelParameteriv(_target, level, pname, &value);
	CheckGLError();

	return value;
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

const char* Texture::typeName() const
{
	return "Texture";
}

void Texture::accept(ObjectVisitor& visitor)
{
	visitor.visitTexture(this);
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

Texture::Handle::Handle()
: value(0)
{
}

Texture::Handle::Handle(const GLuint64& value)
: value(value)
{
}

Texture::Handle::operator GLuint64() const
{
	return value;
}

} // namespace glow
