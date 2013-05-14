#include <glow/VertexArrayObject.h>

using namespace glow;


VertexArrayObject::VertexArrayObject()
: _nextBindingIndex(0)
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

VertexBuffer* VertexArrayObject::createVertexBuffer(const std::string& name)
{
	VertexBuffer* vbo = VertexBuffer::create(this, _nextBindingIndex++);
	_vertexBuffers[name] = vbo;
	return vbo;
}

VertexBuffer* VertexArrayObject::vertexBuffer(const std::string& name)
{
	return _vertexBuffers[name];
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

VertexAttributeBinding* VertexArrayObject::binding(GLint attributeLocation)
{
	if (!_bindings[attributeLocation])
	{
		_bindings[attributeLocation] = new VertexAttributeBinding(this, attributeLocation);
	}

	return _bindings[attributeLocation];
}
