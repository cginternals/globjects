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

namespace {
    glo::AbstractVertexAttributeBindingImplementation & implementation()
    {
        return glo::ImplementationRegistry::current().attributeImplementation();
    }
}

namespace glo
{

VertexArray::VertexArray()
: Object(new VertexArrayObjectResource)
{
}

VertexArray::VertexArray(IDResource * resource)
: Object(resource)
{
}

void VertexArray::hintAttributeImplementation(AttributeImplementation impl)
{
    ImplementationRegistry::current().initialize(impl);
}

VertexArray * VertexArray::fromId(gl::GLuint id)
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
	gl::glBindVertexArray(id());
}

void VertexArray::unbind()
{
	gl::glBindVertexArray(0);
}

VertexAttributeBinding* VertexArray::binding(gl::GLuint bindingIndex)
{
	if (!m_bindings[bindingIndex])
	{
        m_bindings[bindingIndex] = new VertexAttributeBinding(this, bindingIndex);
	}

	return m_bindings[bindingIndex];
}

const VertexAttributeBinding* VertexArray::binding(gl::GLuint bindingIndex) const
{
    if (!m_bindings.count(bindingIndex))
    {
        return nullptr;
    }

    return m_bindings.at(bindingIndex);
}

void VertexArray::enable(gl::GLint attributeIndex)
{
    implementation().enable(this, attributeIndex);
}

void VertexArray::disable(gl::GLint attributeIndex)
{
    implementation().disable(this, attributeIndex);
}

std::vector<VertexAttributeBinding*> VertexArray::bindings()
{
	std::vector<VertexAttributeBinding*> bindings;

    for (std::pair<gl::GLuint, ref_ptr<VertexAttributeBinding>> pair: m_bindings)
	{
		bindings.push_back(pair.second);
	}

	return bindings;
}

std::vector<const VertexAttributeBinding*> VertexArray::bindings() const
{
    std::vector<const VertexAttributeBinding*> bindings;

    for (std::pair<gl::GLuint, ref_ptr<VertexAttributeBinding>> pair: m_bindings)
    {
        bindings.push_back(pair.second);
    }

    return bindings;
}

void VertexArray::drawArrays(gl::GLenum mode, gl::GLint first, gl::GLsizei count) const
{
    bind();
    gl::glDrawArrays(mode, first, count);
}

void VertexArray::drawArraysInstanced(gl::GLenum mode, gl::GLint first, gl::GLsizei count, gl::GLsizei instanceCount) const
{
    bind();
    gl::glDrawArraysInstanced(mode, first, count, instanceCount);
}

void VertexArray::drawArraysInstancedBaseInstance(gl::GLenum mode, gl::GLint first, gl::GLsizei count, gl::GLsizei instanceCount, gl::GLuint baseInstance) const
{
    bind();
    gl::glDrawArraysInstancedBaseInstance(mode, first, count, instanceCount, baseInstance);
}

void VertexArray::drawArraysIndirect(gl::GLenum mode, const void* indirect) const
{
    assert(indirect != nullptr);

    bind();
    gl::glDrawArraysIndirect(mode, indirect);
}

void VertexArray::multiDrawArrays(gl::GLenum mode, gl::GLint* first, const gl::GLsizei* count, gl::GLsizei drawCount) const
{
    bind();
    gl::glMultiDrawArrays(mode, first, count, drawCount);
}

void VertexArray::multiDrawArraysIndirect(gl::GLenum mode, const void* indirect, gl::GLsizei drawCount, gl::GLsizei stride) const
{
    bind();
    gl::glMultiDrawArraysIndirect(mode, indirect, drawCount, stride);
}

void VertexArray::drawElements(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices) const
{
    bind();
    gl::glDrawElements(mode, count, type, indices);
}

void VertexArray::drawElementsBaseVertex(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void* indices, gl::GLint baseVertex) const
{
    bind();
    gl::glDrawElementsBaseVertex(mode, count, type, const_cast<void*>(indices), baseVertex);
}

void VertexArray::drawElementsInstanced(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void* indices, gl::GLsizei primitiveCount) const
{
    bind();
    gl::glDrawElementsInstanced(mode, count, type, indices, primitiveCount);
}

void VertexArray::drawElementsInstancedBaseInstance(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void* indices, gl::GLsizei instanceCount, gl::GLuint baseInstance) const
{
    bind();
    gl::glDrawElementsInstancedBaseInstance(mode, count, type, indices, instanceCount, baseInstance);
}

void VertexArray::drawElementsInstancedBaseVertex(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void* indices, gl::GLsizei instanceCount, gl::GLint baseVertex) const
{
    bind();
    gl::glDrawElementsInstancedBaseVertex(mode, count, type, indices, instanceCount, baseVertex);
}

void VertexArray::drawElementsInstancedBaseVertexBaseInstance(gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void* indices, gl::GLsizei instanceCount, gl::GLint baseVertex, gl::GLuint baseInstance) const
{
    bind();
    gl::glDrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instanceCount, baseVertex, baseInstance);
}

void VertexArray::multiDrawElements(gl::GLenum mode, const gl::GLsizei* count, gl::GLenum type, const void** indices, gl::GLsizei drawCount) const
{
    bind();
    gl::glMultiDrawElements(mode, count, type, indices, drawCount);
}

void VertexArray::multiDrawElementsBaseVertex(gl::GLenum mode, const gl::GLsizei* count, gl::GLenum type, const void** indices, gl::GLsizei drawCount, gl::GLint* baseVertex) const
{
    bind();
    gl::glMultiDrawElementsBaseVertex(mode, const_cast<gl::GLsizei*>(count), type, const_cast<void**>(indices), drawCount, baseVertex);
}

void VertexArray::multiDrawElementsIndirect(gl::GLenum mode, gl::GLenum type, const void* indirect, gl::GLsizei drawCount, gl::GLsizei stride) const
{
    bind();
    gl::glMultiDrawElementsIndirect(mode, type, indirect, drawCount, stride);
}

void VertexArray::drawRangeElements(gl::GLenum mode, gl::GLuint start, gl::GLuint end, gl::GLsizei count, gl::GLenum type, const void* indices) const
{
    bind();
    gl::glDrawRangeElements(mode, start, end, count, type, indices);
}

void VertexArray::drawRangeElementsBaseVertex(gl::GLenum mode, gl::GLuint start, gl::GLuint end, gl::GLsizei count, gl::GLenum type, const void* indices, gl::GLint baseVertex) const
{
    bind();
    gl::glDrawRangeElementsBaseVertex(mode, start, end, count, type, const_cast<void*>(indices), baseVertex);
}

void VertexArray::multiDrawArrays(gl::GLenum mode, const std::vector<VertexArray::MultiDrawArraysRange> & ranges) const
{
    std::vector<gl::GLint> firsts = collect_member(ranges, MultiDrawArraysRange, first);
    std::vector<gl::GLsizei> counts = collect_member(ranges, MultiDrawArraysRange, count);

    multiDrawArrays(mode, firsts.data(), counts.data(), static_cast<gl::GLsizei>(ranges.size()));
}

void VertexArray::multiDrawElements(gl::GLenum mode, gl::GLenum type, const std::vector<VertexArray::MultiDrawElementsRange> & ranges) const
{
    std::vector<gl::GLsizei> counts = collect_member(ranges, MultiDrawElementsRange, count);
    std::vector<const void*> indices = collect_type_member(const void*, ranges, MultiDrawElementsRange, indices);

    multiDrawElements(mode, counts.data(), type, indices.data(), static_cast<gl::GLsizei>(ranges.size()));
}

void VertexArray::multiDrawElementsBaseVertex(gl::GLenum mode, gl::GLenum type, const std::vector<VertexArray::MultiDrawElementsBaseVertexRange> & ranges) const
{
    std::vector<gl::GLsizei> counts = collect_member(ranges, MultiDrawElementsBaseVertexRange, count);
    std::vector<const void*> indices = collect_type_member(const void*, ranges, MultiDrawElementsBaseVertexRange, indices);
    std::vector<gl::GLint> baseVertices = collect_member(ranges, MultiDrawElementsBaseVertexRange, baseVertex);

    multiDrawElementsBaseVertex(mode, counts.data(), type, indices.data(), static_cast<gl::GLsizei>(ranges.size()), baseVertices.data());
}

gl::GLenum VertexArray::objectType() const
{
    return gl::GL_VERTEX_ARRAY;
}

} // namespace glo
