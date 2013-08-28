#pragma once

#include <glow/glow.h>
#include <glow/Object.h>

// http://www.opengl.org/wiki/Texture

namespace glow {

class GLOW_API Texture : public Object
{
public:
	class Handle
	{
	public:
		Handle();
		Handle(const GLuint64& value);

		operator GLuint64() const;

		GLuint64 value;
	};
public:
	Texture(GLenum  target = GL_TEXTURE_2D);
	Texture(GLuint id, GLenum  target, bool ownsGLObject = true);
	virtual ~Texture();

	virtual const char* typeName() const;

	void setParameter(GLenum name, GLint value);
	void setParameter(GLenum name, GLfloat value);

	void bind();
	void unbind();

	GLenum target() const;

	void image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data);
	void storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);

	void bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);

	void generateMipmap();

	Handle textureHandle() const;

	GLboolean isResident() const;
	Handle makeResident();
	void makeNonResident();
protected:
	GLenum  _target;

	static GLuint genTexture();
};

} // namespace glow
