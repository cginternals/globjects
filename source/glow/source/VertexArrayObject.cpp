#include <glow/VertexArrayObject.h>

#include <cassert>

#include <glow/Error.h>
#include <glow/ObjectVisitor.h>
#include <glow/VertexAttributeBinding.h>

#include "container_helpers.hpp"

namespace glow
{

VertexArrayObject::VertexArrayObject()
: Object(genVertexArray())
{
}

VertexArrayObject::VertexArrayObject(GLuint id, bool ownsGLObject)
: Object(id, ownsGLObject)
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

void VertexArrayObject::accept(ObjectVisitor & visitor)
{
	visitor.visitVertexArrayObject(this);
}

void VertexArrayObject::bind() const
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
        m_bindings[bindingIndex] = new VertexAttributeBinding(this, bindingIndex);
	}

	return m_bindings[bindingIndex];
}

const VertexAttributeBinding* VertexArrayObject::binding(GLuint bindingIndex) const
{
    if (!m_bindings.count(bindingIndex))
    {
        return nullptr;
    }

    return m_bindings.at(bindingIndex);
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

void VertexArrayObject::setAttributeDivisor(GLint attributeIndex, GLuint divisor)
{
    bind();

    glVertexAttribDivisor(attributeIndex, divisor);
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

std::vector<const VertexAttributeBinding*> VertexArrayObject::bindings() const
{
    std::vector<const VertexAttributeBinding*> bindings;

    for (std::pair<GLuint, ref_ptr<VertexAttributeBinding>> pair: m_bindings)
    {
        bindings.push_back(pair.second);
    }

    return bindings;
}

void VertexArrayObject::drawArrays(GLenum mode, GLint first, GLsizei count) const
{
    bind();
    glDrawArrays(mode, first, count);
    CheckGLError();
}

void VertexArrayObject::drawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount) const
{
    bind();
    glDrawArraysInstanced(mode, first, count, instanceCount);
    CheckGLError();
}

void VertexArrayObject::drawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount, GLuint baseInstance) const
{
    bind();
    glDrawArraysInstancedBaseInstance(mode, first, count, instanceCount, baseInstance);
    CheckGLError();
}

void VertexArrayObject::drawArraysIndirect(GLenum mode, const void* indirect) const
{
    assert(indirect != nullptr);

    bind();
    glDrawArraysIndirect(mode, indirect);
    CheckGLError();
}

void VertexArrayObject::multiDrawArrays(GLenum mode, GLint* first, const GLsizei* count, GLsizei drawCount) const
{
    bind();
    glMultiDrawArrays(mode, first, count, drawCount);
    CheckGLError();
}

void VertexArrayObject::multiDrawArraysIndirect(GLenum mode, const void* indirect, GLsizei drawCount, GLsizei stride) const
{
    bind();
    glMultiDrawArraysIndirect(mode, indirect, drawCount, stride);
    CheckGLError();
}

void VertexArrayObject::drawElements(GLenum mode, GLsizei count, GLenum type, const void * indices) const
{
    bind();
    glDrawElements(mode, count, type, indices);
    CheckGLError();
}

void VertexArrayObject::drawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLint baseVertex) const
{
    bind();
    glDrawElementsBaseVertex(mode, count, type, const_cast<void*>(indices), baseVertex);
    CheckGLError();
}

void VertexArrayObject::drawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primitiveCount) const
{
    bind();
    glDrawElementsInstanced(mode, count, type, indices, primitiveCount);
    CheckGLError();
}

void VertexArrayObject::drawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instanceCount, GLuint baseInstance) const
{
    bind();
    glDrawElementsInstancedBaseInstance(mode, count, type, indices, instanceCount, baseInstance);
    CheckGLError();
}

void VertexArrayObject::drawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instanceCount, GLint baseVertex) const
{
    bind();
    glDrawElementsInstancedBaseVertex(mode, count, type, indices, instanceCount, baseVertex);
    CheckGLError();
}

void VertexArrayObject::drawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instanceCount, GLint baseVertex, GLuint baseInstance) const
{
    bind();
    glDrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instanceCount, baseVertex, baseInstance);
    CheckGLError();
}

void VertexArrayObject::multiDrawElements(GLenum mode, const GLsizei* count, GLenum type, const void** indices, GLsizei drawCount) const
{
    bind();
    glMultiDrawElements(mode, count, type, indices, drawCount);
    CheckGLError();
}

void VertexArrayObject::multiDrawElementsBaseVertex(GLenum mode, const GLsizei* count, GLenum type, const void** indices, GLsizei drawCount, GLint* baseVertex) const
{
    bind();
    glMultiDrawElementsBaseVertex(mode, const_cast<GLsizei*>(count), type, const_cast<void**>(indices), drawCount, baseVertex);
    CheckGLError();
}

void VertexArrayObject::multiDrawElementsIndirect(GLenum mode, GLenum type, const void* indirect, GLsizei drawCount, GLsizei stride) const
{
    bind();
    glMultiDrawElementsIndirect(mode, type, indirect, drawCount, stride);
    CheckGLError();
}

void VertexArrayObject::drawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices) const
{
    bind();
    glDrawRangeElements(mode, start, end, count, type, indices);
    CheckGLError();
}

void VertexArrayObject::drawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices, GLint baseVertex) const
{
    bind();
    glDrawRangeElementsBaseVertex(mode, start, end, count, type, const_cast<void*>(indices), baseVertex);
    CheckGLError();
}

void VertexArrayObject::multiDrawArrays(GLenum mode, const std::vector<VertexArrayObject::MultiDrawArraysRange> & ranges) const
{
    std::vector<GLint> firsts = collect_member(ranges, MultiDrawArraysRange, first);
    std::vector<GLsizei> counts = collect_member(ranges, MultiDrawArraysRange, count);

    multiDrawArrays(mode, firsts.data(), counts.data(), static_cast<GLsizei>(ranges.size()));
}

void VertexArrayObject::multiDrawElements(GLenum mode, GLenum type, const std::vector<VertexArrayObject::MultiDrawElementsRange> & ranges) const
{
    std::vector<GLsizei> counts = collect_member(ranges, MultiDrawElementsRange, count);
    std::vector<const void*> indices = collect_type_member(const void*, ranges, MultiDrawElementsRange, indices);

    multiDrawElements(mode, counts.data(), type, indices.data(), static_cast<GLsizei>(ranges.size()));
}

void VertexArrayObject::multiDrawElementsBaseVertex(GLenum mode, GLenum type, const std::vector<VertexArrayObject::MultiDrawElementsBaseVertexRange> & ranges) const
{
    std::vector<GLsizei> counts = collect_member(ranges, MultiDrawElementsBaseVertexRange, count);
    std::vector<const void*> indices = collect_type_member(const void*, ranges, MultiDrawElementsBaseVertexRange, indices);
    std::vector<GLint> baseVertices = collect_member(ranges, MultiDrawElementsBaseVertexRange, baseVertex);

    multiDrawElementsBaseVertex(mode, counts.data(), type, indices.data(), static_cast<GLsizei>(ranges.size()), baseVertices.data());
}

} // namespace glow
