#include <glow/VertexArrayObject.h>

using namespace glow;

VertexAttributeArray::VertexAttributeArray(GLuint index, VertexArrayObject* vao)
: Buffer(GL_ARRAY_BUFFER)
, _index(index)
, _vao(vao)
{
}

void VertexAttributeArray::setData(ArrayData* data, GLenum usage, GLboolean normalized, GLsizei stride, const GLvoid* offset)
{
	Buffer::setData(data, usage);
	_vao->bind();
	bind();
	glVertexAttribPointer(_index, data->elementSize(), data->type(), normalized, stride, offset);
	unbind();
	_vao->unbind();
}

void VertexAttributeArray::setDataI(ArrayData* data, GLenum usage, GLsizei stride, const GLvoid* offset)
{
	Buffer::setData(data, usage);
	_vao->bind();
	bind();
	glVertexAttribIPointer(_index, data->elementSize(), data->type(), stride, offset);
	unbind();
	_vao->unbind();
}

void VertexAttributeArray::setDataL(ArrayData* data, GLenum usage, GLsizei stride, const GLvoid* offset)
{
	Buffer::setData(data, usage);
	_vao->bind();
	bind();
	glVertexAttribLPointer(_index, data->elementSize(), data->type(), stride, offset);
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


VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &_id);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &_id);
}

void VertexArrayObject::bind()
{
	glBindVertexArray(_id);
}

void VertexArrayObject::unbind()
{
	glBindVertexArray(0);
}

VertexAttributeArray* VertexArrayObject::createAttributeArray(const std::string& name, GLuint index)
{
	bind();
	VertexAttributeArray* attributeArray = new VertexAttributeArray(index, this);
	_attributeArrays[name] = attributeArray;
	unbind();
	return attributeArray;
}

VertexAttributeArray* VertexArrayObject::attributeArray(const std::string& name)
{
	return _attributeArrays[name];
}

Buffer* VertexArrayObject::createElementBuffer(const std::string& name)
{
	bind();
	Buffer* buffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER);
	_indexBuffers[name] = buffer;
	unbind();
	return buffer;
}

Buffer* VertexArrayObject::elementBuffer(const std::string& name)
{
	return _indexBuffers[name];
}
