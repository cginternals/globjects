#include <glow/VertexBuffer.h>
#include <glow/VertexArrayObject.h>

using namespace glow;


VertexBuffer::VertexBuffer(VertexArrayObject* vao, GLint bindingIndex)
: Buffer(GL_ARRAY_BUFFER)
, _vao(vao)
, _bindingIndex(bindingIndex)
, _baseOffset(0)
, _stride(0)
{
}

VertexBuffer* VertexBuffer::create(VertexArrayObject* vao, GLint bindingIndex)
{
	vao->bind();
	VertexBuffer* vbo = new VertexBuffer(vao, bindingIndex);
	vao->unbind();
	return vbo;
}

GLint VertexBuffer::bindingIndex() const
{
	return _bindingIndex;
}

VertexArrayObject* VertexBuffer::vao()
{
	return _vao;
}

void VertexBuffer::setBaseOffset(GLintptr baseOffset, GLintptr stride)
{
	_baseOffset = baseOffset;
	_stride = stride;
}

GLintptr VertexBuffer::baseOffset() const
{
	return _baseOffset;
}

GLintptr VertexBuffer::stride() const
{
	return _stride == 0 ? _data->sizeofElement() : _stride;
}

void VertexBuffer::bindVertexBuffer()
{
	glBindVertexBuffer(_bindingIndex, _id, _baseOffset, stride());
}
