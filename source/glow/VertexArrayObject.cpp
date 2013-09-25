
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

void VertexArrayObject::drawArrays(GLenum mode, GLint first, GLsizei count)
{
    bind();
    glDrawArrays(mode, first, count);
    CheckGLError();
}

void VertexArrayObject::drawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount)
{
    bind();
    glDrawArraysInstanced(mode, first, count, instanceCount);
    CheckGLError();
}

void VertexArrayObject::drawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount, GLuint baseInstance)
{
    bind();
    glDrawArraysInstancedBaseInstance(mode, first, count, instanceCount, baseInstance);
    CheckGLError();
}

void VertexArrayObject::drawArraysIndirect(GLenum mode, const void* indirect)
{
    bind();
    glDrawArraysIndirect(mode, indirect);
    CheckGLError();
}

void VertexArrayObject::multiDrawArrays(GLenum mode, GLint* first, const GLsizei* count, GLsizei drawCount)
{
    bind();
    glMultiDrawArrays(mode, first, count, drawCount);
    CheckGLError();
}

void VertexArrayObject::multiDrawArraysIndirect(GLenum mode, const void* indirect, GLsizei drawCount, GLsizei stride)
{
    bind();
    glMultiDrawArraysIndirect(mode, indirect, drawCount, stride);
    CheckGLError();
}

void VertexArrayObject::drawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
    bind();
    glDrawElements(mode, count, type, indices);
    CheckGLError();
}

void VertexArrayObject::drawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLint baseVertex)
{
    bind();
    glDrawElementsBaseVertex(mode, count, type, const_cast<void*>(indices), baseVertex);
    CheckGLError();
}

void VertexArrayObject::drawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primitiveCount)
{
    bind();
    glDrawElementsInstanced(mode, count, type, indices, primitiveCount);
    CheckGLError();
}

void VertexArrayObject::drawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instanceCount, GLuint baseInstance)
{
    bind();
    glDrawElementsInstancedBaseInstance(mode, count, type, indices, instanceCount, baseInstance);
    CheckGLError();
}

void VertexArrayObject::drawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instanceCount, GLint baseVertex)
{
    bind();
    glDrawElementsInstancedBaseVertex(mode, count, type, indices, instanceCount, baseVertex);
    CheckGLError();
}

void VertexArrayObject::drawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instanceCount, GLint baseVertex, GLuint baseInstance)
{
    bind();
    glDrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instanceCount, baseVertex, baseInstance);
    CheckGLError();
}

void VertexArrayObject::multiDrawElements(GLenum mode, const GLsizei* count, GLenum type, const void** indices, GLsizei drawCount)
{
    bind();
    glMultiDrawElements(mode, count, type, indices, drawCount);
    CheckGLError();
}

void VertexArrayObject::multiDrawElementsBaseVertex(GLenum mode, const GLsizei* count, GLenum type, const void** indices, GLsizei primitiveCount, GLint* baseVertex)
{
    bind();
    glMultiDrawElementsBaseVertex(mode, const_cast<GLsizei*>(count), type, const_cast<void**>(indices), primitiveCount, baseVertex);
    CheckGLError();
}

void VertexArrayObject::multiDrawElementsIndirect(GLenum mode, GLenum type, const void* indirect, GLsizei drawCount, GLsizei stride)
{
    bind();
    glMultiDrawElementsIndirect(mode, type, indirect, drawCount, stride);
    CheckGLError();
}

void VertexArrayObject::drawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices)
{
    bind();
    glDrawRangeElements(mode, start, end, count, type, indices);
    CheckGLError();
}

void VertexArrayObject::drawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices, GLint baseVertex)
{
    bind();
    glDrawRangeElementsBaseVertex(mode, start, end, count, type, const_cast<void*>(indices), baseVertex);
    CheckGLError();
}


} // namespace glow
