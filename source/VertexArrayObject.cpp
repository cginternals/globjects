#include <glow/VertexArrayObject.h>

using namespace glow;

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &_id);
}

VertexArrayObject::~VertexArrayObject()
{
	for (Buffer* buffer: _buffers)
	{
		delete buffer;
	}
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

Buffer* VertexArrayObject::addArrayBuffer()
{
	return createBuffer(GL_ARRAY_BUFFER);
}

Buffer* VertexArrayObject::addElementBuffer()
{
	return createBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

Buffer* VertexArrayObject::createBuffer(GLenum target)
{
	Buffer* buffer = new Buffer(target);
	_buffers.push_back(buffer);
	return buffer;
}
