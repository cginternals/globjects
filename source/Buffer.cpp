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

void Buffer::setData(ArrayData* data, GLenum usage)
{
	_data = data;
	bind();
	glBufferData(_target, data->byteSize(), data->rawData(), usage);
	unbind();
}

ArrayData* Buffer::data()
{
	return _data;
}

void Buffer::drawArrays(GLenum mode, GLint first, GLsizei count)
{
	bind();
	glDrawArrays(mode, first, count<0?_data->elementCount():count);
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
