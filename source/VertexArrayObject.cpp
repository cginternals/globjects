#include <glow/VertexArrayObject.h>

using namespace glow;

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &_id);
}

VertexArrayObject::~VertexArrayObject()
{
	for (std::pair<std::string, Buffer*> bufferPair: _buffers)
	{
		delete bufferPair.second;
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

Buffer* VertexArrayObject::addArrayBuffer(const std::string& name)
{
	return createBuffer(name, GL_ARRAY_BUFFER);
}

Buffer* VertexArrayObject::addElementBuffer(const std::string& name)
{
	return createBuffer(name, GL_ELEMENT_ARRAY_BUFFER);
}

Buffer* VertexArrayObject::buffer(const std::string& name)
{
	return _buffers[name];
}

Buffer* VertexArrayObject::createBuffer(const std::string& name, GLenum target)
{
	Buffer* buffer = new Buffer(target);
	_buffers[name] = buffer;
	return buffer;
}
