#include <glow/Buffer.h>

#include <iostream>

using namespace glow;


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
