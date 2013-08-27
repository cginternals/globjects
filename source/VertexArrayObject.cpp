
#include <glow/VertexArrayObject.h>

#include <glow/Error.h>

namespace glow
{

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
		glDeleteVertexArrays(1, &m_id);
		CheckGLError();
	}
}

GLuint VertexArrayObject::genVertexArray()
{
	GLuint id = 0;

	glGenVertexArrays(1, &id);
	CheckGLError();
	return id;
}

const char* VertexArrayObject::typeName() const
{
	return "VertexArrayObject";
}

void VertexArrayObject::bind()
{
	glBindVertexArray(m_id);
	CheckGLError();
}

void VertexArrayObject::unbind()
{
	glBindVertexArray(0);
	CheckGLError();
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
	CheckGLError();
}

void VertexArrayObject::disable(GLint attributeIndex)
{
	bind();

	glDisableVertexAttribArray(attributeIndex);
	CheckGLError();
}

} // namespace glow
