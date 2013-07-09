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

VertexAttributeBinding* VertexArrayObject::binding(GLuint bindingIndex)
{
	if (!_bindings[bindingIndex])
	{
		_bindings[bindingIndex] = new VertexAttributeBinding(this, _nextBindingIndex++);
	}

	return _bindings[bindingIndex];
}

void VertexArrayObject::enable(GLint attributeIndex)
{
	bind();
	glEnableVertexAttribArray(attributeIndex);
}

void VertexArrayObject::disable(GLint attributeIndex)
{
	bind();
	glDisableVertexAttribArray(attributeIndex);
}
