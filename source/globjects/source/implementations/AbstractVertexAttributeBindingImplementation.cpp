
#include "AbstractVertexAttributeBindingImplementation.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/extension.h>

#include <globjects/globjects.h>

#include "VertexAttributeBindingImplementation_DirectStateAccessARB.h"
#include "VertexAttributeBindingImplementation_VertexAttribBindingARB.h"
#include "VertexAttributeBindingImplementation_Legacy.h"

#include <globjects/VertexAttributeBinding.h>


using namespace gl;


namespace globjects 
{


AbstractVertexAttributeBindingImplementation::AbstractVertexAttributeBindingImplementation()
{
}

AbstractVertexAttributeBindingImplementation::~AbstractVertexAttributeBindingImplementation()
{
}

AbstractVertexAttributeBindingImplementation * AbstractVertexAttributeBindingImplementation::get(const VertexArray::AttributeImplementation impl)
{
    if (impl == VertexArray::AttributeImplementation::DirectStateAccessARB
     && hasExtension(GLextension::GL_ARB_direct_state_access))
    {
        return VertexAttributeBindingImplementation_DirectStateAccessARB::instance();
    }
    else if (impl == VertexArray::AttributeImplementation::VertexAttribBindingARB
     && hasExtension(GLextension::GL_ARB_vertex_attrib_binding))
    {
        return VertexAttributeBindingImplementation_VertexAttribBindingARB::instance();
    }
    else
    {
        return VertexAttributeBindingImplementation_Legacy::instance();
    }
}

GLint AbstractVertexAttributeBindingImplementation::attributeIndex(const VertexAttributeBinding * binding) const
{
    return binding->attributeIndex();
}

GLint AbstractVertexAttributeBindingImplementation::bindingIndex(const VertexAttributeBinding * binding) const
{
    return binding->bindingIndex();
}

const VertexArray * AbstractVertexAttributeBindingImplementation::vao(const VertexAttributeBinding * binding) const
{
    return binding->vao();
}

const Buffer * AbstractVertexAttributeBindingImplementation::vbo(const VertexAttributeBinding * binding) const
{
    return binding->buffer();
}

void * & AbstractVertexAttributeBindingImplementation::bindingData(const VertexAttributeBinding * binding) const
{
    return binding->m_bindingData;
}

void AbstractVertexAttributeBindingImplementation::bind(const VertexArray * vertexArray) const
{
    glBindVertexArray(vertexArray->id());
}

void AbstractVertexAttributeBindingImplementation::unbind() const
{
    glBindVertexArray(0);
}

void AbstractVertexAttributeBindingImplementation::drawArrays(const VertexArray * vertexArray, gl::GLenum mode, gl::GLint first, gl::GLsizei count) const
{
    bind(vertexArray);
    glDrawArrays(mode, first, count);
}

void AbstractVertexAttributeBindingImplementation::drawArraysInstanced(const VertexArray * vertexArray, gl::GLenum mode, gl::GLint first, gl::GLsizei count, gl::GLsizei instanceCount) const
{
    bind(vertexArray);
    glDrawArraysInstanced(mode, first, count, instanceCount);
}

void AbstractVertexAttributeBindingImplementation::drawArraysInstancedBaseInstance(const VertexArray * vertexArray, gl::GLenum mode, gl::GLint first, gl::GLsizei count, gl::GLsizei instanceCount, gl::GLuint baseInstance) const
{
    bind(vertexArray);
    glDrawArraysInstancedBaseInstance(mode, first, count, instanceCount, baseInstance);
}

void AbstractVertexAttributeBindingImplementation::drawArraysIndirect(const VertexArray * vertexArray, gl::GLenum mode, const void * indirect) const
{
    bind(vertexArray);
    glDrawArraysIndirect(mode, indirect);
}

void AbstractVertexAttributeBindingImplementation::multiDrawArrays(const VertexArray * vertexArray, gl::GLenum mode, gl::GLint * first, const gl::GLsizei * count, gl::GLsizei drawCount) const
{
    bind(vertexArray);
    glMultiDrawArrays(mode, first, count, drawCount);
}

void AbstractVertexAttributeBindingImplementation::multiDrawArraysIndirect(const VertexArray * vertexArray, gl::GLenum mode, const void * indirect, gl::GLsizei drawCount, gl::GLsizei stride) const
{
    bind(vertexArray);
    glMultiDrawArraysIndirect(mode, indirect, drawCount, stride);
}

void AbstractVertexAttributeBindingImplementation::drawElements(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices) const
{
    bind(vertexArray);
    glDrawElements(mode, count, type, indices);
}

void AbstractVertexAttributeBindingImplementation::drawElementsBaseVertex(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLint baseVertex) const
{
    bind(vertexArray);
    glDrawElementsBaseVertex(mode, count, type, const_cast<void*>(indices), baseVertex);
}

void AbstractVertexAttributeBindingImplementation::drawElementsInstanced(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei primitiveCount) const
{
    bind(vertexArray);
    glDrawElementsInstanced(mode, count, type, indices, primitiveCount);
}

void AbstractVertexAttributeBindingImplementation::drawElementsInstancedBaseInstance(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei instanceCount, gl::GLuint baseInstance) const
{
    bind(vertexArray);
    glDrawElementsInstancedBaseInstance(mode, count, type, indices, instanceCount, baseInstance);
}

void AbstractVertexAttributeBindingImplementation::drawElementsInstancedBaseVertex(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei instanceCount, gl::GLint baseVertex) const
{
    bind(vertexArray);
    glDrawElementsInstancedBaseVertex(mode, count, type, indices, instanceCount, baseVertex);
}

void AbstractVertexAttributeBindingImplementation::drawElementsInstancedBaseVertexBaseInstance(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei instanceCount, gl::GLint baseVertex, gl::GLuint baseInstance) const
{
    bind(vertexArray);
    glDrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instanceCount, baseVertex, baseInstance);
}

void AbstractVertexAttributeBindingImplementation::multiDrawElements(const VertexArray * vertexArray, gl::GLenum mode, const gl::GLsizei * count, gl::GLenum type, const void ** indices, gl::GLsizei drawCount) const
{
    bind(vertexArray);
    glMultiDrawElements(mode, count, type, indices, drawCount);
}

void AbstractVertexAttributeBindingImplementation::multiDrawElementsBaseVertex(const VertexArray * vertexArray, gl::GLenum mode, const gl::GLsizei * count, gl::GLenum type, const void ** indices, gl::GLsizei drawCount, gl::GLint * baseVertex) const
{
    bind(vertexArray);
    glMultiDrawElementsBaseVertex(mode, const_cast<GLsizei*>(count), type, const_cast<void**>(indices), drawCount, baseVertex);
}

void AbstractVertexAttributeBindingImplementation::multiDrawElementsIndirect(const VertexArray * vertexArray, gl::GLenum mode, gl::GLenum type, const void * indirect, gl::GLsizei drawCount, gl::GLsizei stride) const
{
    bind(vertexArray);
    glMultiDrawElementsIndirect(mode, type, indirect, drawCount, stride);
}

void AbstractVertexAttributeBindingImplementation::drawRangeElements(const VertexArray * vertexArray, gl::GLenum mode, gl::GLuint start, gl::GLuint end, gl::GLsizei count, gl::GLenum type, const void * indices) const
{
    bind(vertexArray);
    glDrawRangeElements(mode, start, end, count, type, indices);
}

void AbstractVertexAttributeBindingImplementation::drawRangeElementsBaseVertex(const VertexArray * vertexArray, gl::GLenum mode, gl::GLuint start, gl::GLuint end, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLint baseVertex) const
{
    bind(vertexArray);
    glDrawRangeElementsBaseVertex(mode, start, end, count, type, const_cast<void*>(indices), baseVertex);
}


} // namespace globjects
