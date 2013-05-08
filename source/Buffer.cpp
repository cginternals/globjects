#include <glow/Buffer.h>

#include <iostream>

using namespace glow;


Buffer::Buffer(GLenum target)
: _target(target)
, _size(-1)
{
	glGenBuffers(1, &_id);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &_id);
}

GLsizeiptr Buffer::size() const
{
	return _size;
}

void Buffer::bind()
{
	glBindBuffer(_target, _id);
}

void Buffer::unbind()
{
	glBindBuffer(_target, 0);
}

void Buffer::data(GLsizeiptr size, const GLvoid* data, GLenum usage)
{
	bind();
	glBufferData(_target, size, data, usage);
	_size = size;
	unbind();
}

void Buffer::vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
	bind();
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	unbind();
}

void Buffer::drawArrays(GLenum mode, GLint first, GLsizei count)
{
	bind();
	glDrawArrays(mode, first, count<0?_size:count);
	unbind();
}

void Buffer::drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
	bind();
	glDrawElements(mode, count, type, indices);
	unbind();
}
