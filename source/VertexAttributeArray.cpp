#include <glow/VertexAttributeArray.h>
#include <glow/VertexArrayObject.h>

using namespace glow;

VertexAttributeArray::VertexAttributeArray(VertexArrayObject* vao)
: Buffer(GL_ARRAY_BUFFER)
, _vao(vao)
, _index(-1)
{
}

void VertexAttributeArray::bindToIndex(GLuint index, GLboolean normalized, GLsizei stride, const GLvoid* offset)
{
	_index = index;
	_vao->bind();
	bind();
	glVertexAttribPointer(index, _data->elementSize(), _data->type(), normalized, stride, offset);
	unbind();
	_vao->unbind();
}

void VertexAttributeArray::bindToIndexI(GLuint index, GLsizei stride, const GLvoid* offset)
{
	_index = index;
	_vao->bind();
	bind();
	glVertexAttribIPointer(index, _data->elementSize(), _data->type(), stride, offset);
	unbind();
	_vao->unbind();
}

void VertexAttributeArray::bindToIndexL(GLuint index, GLsizei stride, const GLvoid* offset)
{
	_index = index;
	_vao->bind();
	bind();
	glVertexAttribLPointer(index, _data->elementSize(), _data->type(), stride, offset);
	unbind();
	_vao->unbind();
}

void VertexAttributeArray::enable()
{
	_vao->bind();
	bind();
	glEnableVertexAttribArray(_index);
	unbind();
	_vao->unbind();
}

void VertexAttributeArray::disable()
{
	_vao->bind();
	bind();
	glDisableVertexAttribArray(_index);
	unbind();
	_vao->unbind();
}
