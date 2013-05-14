#include <glow/VertexAttributeBinding.h>
#include <glow/VertexArrayObject.h>

using namespace glow;

VertexAttributeBinding::Format::Format()
: type(Normal)
, normalized(GL_FALSE)
, stride(0)
, offset(nullptr)
{
}

VertexAttributeBinding::Format::Format(Type type, GLboolean normalized, GLsizei stride , const GLvoid* offset)
: type(type)
, normalized(normalized)
, stride(stride)
, offset(offset)
{
}

//--------------------

VertexAttributeBinding::VertexAttributeBinding(VertexArrayObject* vao, GLint index)
: _vao(vao)
, _index(index)
, _vbo(nullptr)
{
}

void VertexAttributeBinding::setBuffer(VertexBuffer* vbo)
{
	_vbo = vbo;
	bindIndex();
}

void VertexAttributeBinding::enable()
{
	_vao->bind();
	glEnableVertexAttribArray(_index);
	_vao->unbind();
}

void VertexAttributeBinding::disable()
{
	_vao->bind();
	glDisableVertexAttribArray(_index);
	_vao->unbind();
}

void VertexAttributeBinding::setFormat(GLboolean normalized, GLsizei stride, const GLvoid* offset)
{
	_format = Format(Format::Normal, normalized, stride, offset);
	bindIndex();
}

void VertexAttributeBinding::setFormatL(GLsizei stride, const GLvoid* offset)
{
	_format = Format(Format::L, GL_FALSE, stride, offset);
	bindIndex();
}

void VertexAttributeBinding::setFormatI(GLsizei stride, const GLvoid* offset)
{
	_format = Format(Format::I, GL_FALSE, stride, offset);
	bindIndex();
}

void VertexAttributeBinding::bindIndex()
{
	if (!_vbo) return;

	_vao->bind();
	_vbo->bind();
	switch (_format.type)
	{
		case Format::I:
			glVertexAttribIPointer(_index, _vbo->data()->elementSize(), _vbo->data()->type(), _format.stride, _format.offset);
			break;
		case Format::L:
			glVertexAttribIPointer(_index, _vbo->data()->elementSize(), _vbo->data()->type(), _format.stride, _format.offset);
			break;
		default:
			glVertexAttribPointer(_index, _vbo->data()->elementSize(), _vbo->data()->type(), _format.normalized, _format.stride, _format.offset);
	}
	_vbo->unbind();
	_vao->unbind();
}
