#pragma once

#include <glow/Object.h>


namespace glow {

class Buffer : public Object
{
public:
	Buffer(GLenum target);
	~Buffer();

	void bind();
	void unbind();

	GLsizeiptr size() const;

	void data(GLsizeiptr size, const GLvoid* data, GLenum usage);
	void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const GLvoid* pointer = nullptr);

	void drawArrays(GLenum mode, GLint first = 0, GLsizei count = -1);
	void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices = nullptr);
protected:
	GLenum _target;
	GLsizeiptr _size;
};

} // namespace glow
