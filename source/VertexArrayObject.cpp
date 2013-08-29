
#include <glow/VertexArrayObject.h>
#include <glow/Error.h>
#include <glow/ObjectVisitor.h>

namespace glow
{

VertexArrayObject::VertexArrayObject()
: Object(genVertexArray())
, m_nextBindingIndex(0)
{
}

VertexArrayObject::VertexArrayObject(GLuint id, bool ownsGLObject)
: Object(id, ownsGLObject)
, m_nextBindingIndex(0)
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

void VertexArrayObject::accept(ObjectVisitor& visitor)
{
	visitor.visitVertexArrayObject(this);
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
	if (!m_bindings[bindingIndex])
	{
		m_bindings[bindingIndex] = new VertexAttributeBinding(this, m_nextBindingIndex++);
	}

	return m_bindings[bindingIndex];
}

void VertexArrayObject::enable(GLint attributeIndex)
{
    if (-1 == attributeIndex)
        return;

	bind();

	glEnableVertexAttribArray(attributeIndex);
	CheckGLError();
}

void VertexArrayObject::disable(GLint attributeIndex)
{
    if (-1 == attributeIndex)
        return;

    bind();

	glDisableVertexAttribArray(attributeIndex);
	CheckGLError();
}

std::vector<VertexAttributeBinding*> VertexArrayObject::bindings()
{
	std::vector<VertexAttributeBinding*> bindings;

	for (std::pair<GLuint, ref_ptr<VertexAttributeBinding>> pair: m_bindings)
	{
		bindings.push_back(pair.second);
	}

	return bindings;
}

} // namespace glow
