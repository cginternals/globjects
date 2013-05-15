#pragma once

#include <glow/Object.h>

// http://www.opengl.org/wiki/Texture

namespace glow {

class Texture : public Object
{
public:
	Texture(GLenum  target = GL_TEXTURE_2D);
	~Texture();

	void setParameter(GLenum name,  GLint value);
	void setParameter(GLenum name,  GLfloat value);

	void bind();
	void unbind();

	void image2D(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data);
	void storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);

	void bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);

	void generateMipmap();
protected:
	GLenum  _target;
};

} // namespace glow
