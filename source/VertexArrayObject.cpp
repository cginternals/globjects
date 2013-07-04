#include <glow/VertexArrayObject.h>

using namespace glow;


VertexArrayObject::VertexArrayObject()
: Object(genVertexArray())
, _nextBindingIndex(0)
{
}

VertexArrayObject::VertexArrayObject(GLuint id, bool ownsGLObject)
: Object(id, ownsGLObject)
, _nextBindingIndex(0)
{

}

VertexArrayObject::~VertexArrayObject()
{
	if (ownsGLObject())
    {
        glDeleteVertexArrays(1, &_id);
    }
}

GLuint VertexArrayObject::genVertexArray()
{
	GLuint id = 0;
	glGenVertexArrays(1, &id);
	return id;
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
