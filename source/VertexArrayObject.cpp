#include <glow/VertexArrayObject.h>

using namespace glow;


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

VertexAttributeArray* VertexArrayObject::createAttributeArray(const std::string& name)
{
	bind();
	VertexAttributeArray* attributeArray = new VertexAttributeArray(this);
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
