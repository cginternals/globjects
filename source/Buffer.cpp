#include <glow/Buffer.h>

#include <iostream>

using namespace glow;

Buffer::Buffer()
: Object(genBuffer())
, _target(0)
{
}

Buffer::Buffer(GLenum target)
: Object(genBuffer())
, _target(target)
{
}

GLuint Buffer::genBuffer()
{
	GLuint id = 0;
	glGenBuffers(1, &id);
	return id;
}

Buffer::~Buffer()
{
	if (_id) glDeleteBuffers(1, &_id);
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

void* Buffer::map(GLenum access)
{
	bind();
	return glMapBuffer(_target, access);
}

void* Buffer::map(GLenum target, GLenum access)
{
	bind(target);
	return glMapBuffer(target, access);
}

void Buffer::unmap()
{
	glUnmapBuffer(_target);
}

void Buffer::setData(const ArrayData& data, GLenum usage)
{
	setData(data.byteSize(), data.rawData(), usage);
}

void Buffer::setData(GLsizei size, const GLvoid* data, GLenum usage)
{
	bind();
	glBufferData(_target, size, data, usage);
}

void Buffer::drawArrays(GLenum mode, GLint first, GLsizei count)
{
	bind();
	glDrawArrays(mode, first, count);
}

void Buffer::drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
	bind();
	glDrawElements(mode, count, type, indices);
}

void Buffer::bindBase(GLenum target, GLuint index)
{
	glBindBufferBase(target, index, _id);
}

void Buffer::bindRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size)
{
	glBindBufferRange(target, index, _id, offset, size);
}
