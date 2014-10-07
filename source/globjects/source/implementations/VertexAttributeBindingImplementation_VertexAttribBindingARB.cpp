#include "VertexAttributeBindingImplementation_VertexAttribBindingARB.h"

#include <cassert>

#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

#include "VertexAttributeBindingImplementation_Legacy.h"


using namespace gl;

namespace globjects 
{

VertexAttributeBindingImplementation_VertexAttribBindingARB::VertexAttributeBindingImplementation_VertexAttribBindingARB()
{
}

VertexAttributeBindingImplementation_VertexAttribBindingARB::~VertexAttributeBindingImplementation_VertexAttribBindingARB()
{
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::enable(const VertexArray * vertexArray, GLint attributeIndex) const
{
    VertexAttributeBindingImplementation_Legacy::instance()->enable(vertexArray, attributeIndex);
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::disable(const VertexArray * vertexArray, GLint attributeIndex) const
{
    VertexAttributeBindingImplementation_Legacy::instance()->disable(vertexArray, attributeIndex);
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::setAttributeDivisor(const VertexAttributeBinding * binding, GLuint divisor) const
{
    VertexAttributeBindingImplementation_Legacy::instance()->setAttributeDivisor(binding, divisor);
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::bindAttribute(const VertexAttributeBinding * binding, GLint attributeIndex) const
{
    vao(binding)->bind();

    glVertexAttribBinding(attributeIndex, bindingIndex(binding));
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::bindBuffer(const VertexAttributeBinding * binding, const Buffer * vbo, GLint baseoffset, GLint stride) const
{
    vao(binding)->bind();

    glBindVertexBuffer(bindingIndex(binding), vbo ? vbo->id() : 0, baseoffset, stride);
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::setFormat(const VertexAttributeBinding * binding, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) const
{
    vao(binding)->bind();

    glVertexAttribFormat(attributeIndex(binding), size, type, normalized, relativeoffset);
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::setIFormat(const VertexAttributeBinding * binding, GLint size, GLenum type, GLuint relativeoffset) const
{
    vao(binding)->bind();

    glVertexAttribIFormat(attributeIndex(binding), size, type, relativeoffset);
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::setLFormat(const VertexAttributeBinding * binding, GLint size, GLenum type, GLuint relativeoffset) const
{
    vao(binding)->bind();

    glVertexAttribLFormat(attributeIndex(binding), size, type, relativeoffset);
}

} // namespace globjects
