#include <globjects/VertexArray.h>

#include <cassert>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/ObjectVisitor.h>
#include <globjects/VertexAttributeBinding.h>

#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractVertexAttributeBindingImplementation.h"

#include "container_helpers.hpp"
#include "registry/ObjectRegistry.h"

#include "Resource.h"


using namespace gl;

namespace 
{
    globjects::AbstractVertexAttributeBindingImplementation & implementation()
    {
        return globjects::ImplementationRegistry::current().attributeImplementation();
    }
}

namespace globjects
{

VertexArray::VertexArray()
: Object(new VertexArrayObjectResource)
{
}

VertexArray::VertexArray(IDResource * resource)
: Object(resource)
{
}

void VertexArray::hintAttributeImplementation(const AttributeImplementation impl)
{
    ImplementationRegistry::current().initialize(impl);
}

VertexArray * VertexArray::fromId(const GLuint id)
{
    return new VertexArray(new ExternalResource(id));
}

VertexArray * VertexArray::defaultVAO()
{
    return ObjectRegistry::current().defaultVAO();
}

VertexArray::~VertexArray()
{
}

void VertexArray::accept(ObjectVisitor & visitor)
{
    visitor.visitVertexArray(this);
}

void VertexArray::bind() const
{
	glBindVertexArray(id());
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

VertexAttributeBinding * VertexArray::binding(const GLuint bindingIndex)
{
	if (!m_bindings[bindingIndex])
        m_bindings[bindingIndex] = new VertexAttributeBinding(this, bindingIndex);

    return m_bindings[bindingIndex];
}

const VertexAttributeBinding* VertexArray::binding(const GLuint bindingIndex) const
{
    if (m_bindings.count(bindingIndex))
        return m_bindings.at(bindingIndex);

    return nullptr;
}

void VertexArray::enable(GLint attributeIndex)
{
    implementation().enable(this, attributeIndex);
}

void VertexArray::disable(GLint attributeIndex)
{
    implementation().disable(this, attributeIndex);
}

std::vector<VertexAttributeBinding *> VertexArray::bindings()
{
	std::vector<VertexAttributeBinding *> bindings;

    for (std::pair<GLuint, ref_ptr<VertexAttributeBinding>> pair: m_bindings)
		bindings.push_back(pair.second);

	return bindings;
}

std::vector<const VertexAttributeBinding*> VertexArray::bindings() const
{
    std::vector<const VertexAttributeBinding*> bindings;

    for (std::pair<GLuint, ref_ptr<VertexAttributeBinding>> pair: m_bindings)
    {
        bindings.push_back(pair.second);
    }

    return bindings;
}

void VertexArray::drawArrays(const GLenum mode, const GLint first, const GLsizei count) const
{
    bind();
    glDrawArrays(mode, first, count);
}

void VertexArray::drawArraysInstanced(const GLenum mode, const GLint first, const GLsizei count, const GLsizei instanceCount) const
{
    bind();
    glDrawArraysInstanced(mode, first, count, instanceCount);
}

void VertexArray::drawArraysInstancedBaseInstance(const GLenum mode, const GLint first, const GLsizei count, const GLsizei instanceCount, const GLuint baseInstance) const
{
    bind();
    glDrawArraysInstancedBaseInstance(mode, first, count, instanceCount, baseInstance);
}

void VertexArray::drawArraysIndirect(const GLenum mode, const void* indirect) const
{
    assert(indirect != nullptr);

    bind();
    glDrawArraysIndirect(mode, indirect);
}

void VertexArray::multiDrawArrays(const GLenum mode, GLint* first, const GLsizei* count, const GLsizei drawCount) const
{
    bind();
    glMultiDrawArrays(mode, first, count, drawCount);
}

void VertexArray::multiDrawArraysIndirect(const GLenum mode, const void* indirect, const GLsizei drawCount, const GLsizei stride) const
{
    bind();
    glMultiDrawArraysIndirect(mode, indirect, drawCount, stride);
}

void VertexArray::drawElements(const GLenum mode, const GLsizei count, const GLenum type, const void * indices) const
{
    bind();
    glDrawElements(mode, count, type, indices);
}

void VertexArray::drawElementsBaseVertex(const GLenum mode, const GLsizei count, const GLenum type, const void* indices, const GLint baseVertex) const
{
    bind();
    glDrawElementsBaseVertex(mode, count, type, const_cast<void*>(indices), baseVertex);
}

void VertexArray::drawElementsInstanced(const GLenum mode, const GLsizei count, const GLenum type, const void* indices, const GLsizei primitiveCount) const
{
    bind();
    glDrawElementsInstanced(mode, count, type, indices, primitiveCount);
}

void VertexArray::drawElementsInstancedBaseInstance(const GLenum mode, const GLsizei count, const GLenum type, const void* indices, const GLsizei instanceCount, const GLuint baseInstance) const
{
    bind();
    glDrawElementsInstancedBaseInstance(mode, count, type, indices, instanceCount, baseInstance);
}

void VertexArray::drawElementsInstancedBaseVertex(const GLenum mode, const GLsizei count, const GLenum type, const void* indices, const GLsizei instanceCount, const GLint baseVertex) const
{
    bind();
    glDrawElementsInstancedBaseVertex(mode, count, type, indices, instanceCount, baseVertex);
}

void VertexArray::drawElementsInstancedBaseVertexBaseInstance(const GLenum mode, const GLsizei count, const GLenum type, const void* indices, const GLsizei instanceCount, const GLint baseVertex, const GLuint baseInstance) const
{
    bind();
    glDrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instanceCount, baseVertex, baseInstance);
}

void VertexArray::multiDrawElements(const GLenum mode, const GLsizei* count, const GLenum type, const void** indices, const GLsizei drawCount) const
{
    bind();
    glMultiDrawElements(mode, count, type, indices, drawCount);
}

void VertexArray::multiDrawElementsBaseVertex(const GLenum mode, const GLsizei* count, const GLenum type, const void** indices, const GLsizei drawCount, GLint* baseVertex) const
{
    bind();
    glMultiDrawElementsBaseVertex(mode, const_cast<GLsizei*>(count), type, const_cast<void**>(indices), drawCount, baseVertex);
}

void VertexArray::multiDrawElementsIndirect(const GLenum mode, const GLenum type, const void* indirect, const GLsizei drawCount, const GLsizei stride) const
{
    bind();
    glMultiDrawElementsIndirect(mode, type, indirect, drawCount, stride);
}

void VertexArray::drawRangeElements(const GLenum mode, const GLuint start, const GLuint end, const GLsizei count, const GLenum type, const void* indices) const
{
    bind();
    glDrawRangeElements(mode, start, end, count, type, indices);
}

void VertexArray::drawRangeElementsBaseVertex(const GLenum mode, const GLuint start, const GLuint end, const GLsizei count, const GLenum type, const void* indices, const GLint baseVertex) const
{
    bind();
    glDrawRangeElementsBaseVertex(mode, start, end, count, type, const_cast<void*>(indices), baseVertex);
}

void VertexArray::multiDrawArrays(const GLenum mode, const std::vector<VertexArray::MultiDrawArraysRange> & ranges) const
{
    std::vector<GLint> firsts = collect_member(ranges, &MultiDrawArraysRange::first);
    std::vector<GLsizei> counts = collect_member(ranges, &MultiDrawArraysRange::count);

    multiDrawArrays(mode, firsts.data(), counts.data(), static_cast<GLsizei>(ranges.size()));
}

void VertexArray::multiDrawElements(const GLenum mode, const GLenum type, const std::vector<VertexArray::MultiDrawElementsRange> & ranges) const
{
    std::vector<GLsizei> counts = collect_member(ranges, &MultiDrawElementsRange::count);
    std::vector<const void*> indices = collect_type_member<const void*>(ranges, &MultiDrawElementsRange::indices);

    multiDrawElements(mode, counts.data(), type, indices.data(), static_cast<GLsizei>(ranges.size()));
}

void VertexArray::multiDrawElementsBaseVertex(const GLenum mode, const GLenum type, const std::vector<VertexArray::MultiDrawElementsBaseVertexRange> & ranges) const
{
    std::vector<GLsizei> counts = collect_member(ranges, &MultiDrawElementsBaseVertexRange::count);
    std::vector<const void*> indices = collect_type_member<const void*>(ranges, &MultiDrawElementsBaseVertexRange::indices);
    std::vector<GLint> baseVertices = collect_member(ranges, &MultiDrawElementsBaseVertexRange::baseVertex);

    multiDrawElementsBaseVertex(mode, counts.data(), type, indices.data(), static_cast<GLsizei>(ranges.size()), baseVertices.data());
}

GLenum VertexArray::objectType() const
{
    return GL_VERTEX_ARRAY;
}

} // namespace globjects
