#include <glow/Buffer.h>

#include <iostream>

using namespace glow;

Buffer::Buffer()
: _target(0)
{
	glGenBuffers(1, &_id);
}

Buffer::Buffer(GLenum target)
: _target(target)
{
	glGenBuffers(1, &_id);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &_id);
}

void Buffer::bind()
{
	glBindBuffer(_target, _id);
}

void Buffer::bind(GLenum target)
{
	_target = target;
	bind();
}

void Buffer::unbind()
{
	glBindBuffer(_target, 0);
}

void Buffer::setData(const ArrayData& data, GLenum usage)
{
	setData(data.byteSize(), data.rawData(), usage);
}

void Buffer::setData(GLsizei size, const GLvoid* data, GLenum usage)
{
	bind();
	glBufferData(_target, size, data, usage);
	unbind();
}

void Buffer::drawArrays(GLenum mode, GLint first, GLsizei count)
{
	bind();
	glDrawArrays(mode, first, count);
	unbind();
}

void Buffer::drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
	bind();
	glDrawElements(mode, count, type, indices);
	unbind();
}

void Buffer::bindBase(GLenum target, GLuint index)
{
	glBindBufferBase(target, index, _id);
}

void Buffer::bindRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size)
{
	glBindBufferRange(target, index, _id, offset, size);
}
