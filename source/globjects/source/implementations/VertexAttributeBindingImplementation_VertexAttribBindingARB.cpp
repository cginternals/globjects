#include "VertexAttributeBindingImplementation_VertexAttribBindingARB.h"

#include <cassert>

#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

namespace glo {

VertexAttributeBindingImplementation_VertexAttribBindingARB::VertexAttributeBindingImplementation_VertexAttribBindingARB()
{

}

VertexAttributeBindingImplementation_VertexAttribBindingARB::~VertexAttributeBindingImplementation_VertexAttribBindingARB()
{

}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::bindAttribute(const VertexAttributeBinding * binding, gl::GLint attributeIndex) const
{
    vao(binding)->bind();

    gl::glVertexAttribBinding(attributeIndex, bindingIndex(binding));
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::bindBuffer(const VertexAttributeBinding * binding, const Buffer * vbo, gl::GLint baseoffset, gl::GLint stride) const
{
    vao(binding)->bind();

    gl::glBindVertexBuffer(bindingIndex(binding), vbo ? vbo->id() : 0, baseoffset, stride);
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::setFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset) const
{
    vao(binding)->bind();

    gl::glVertexAttribFormat(attributeIndex(binding), size, type, normalized, relativeoffset);
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::setIFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) const
{
    vao(binding)->bind();

    gl::glVertexAttribIFormat(attributeIndex(binding), size, type, relativeoffset);
}

void VertexAttributeBindingImplementation_VertexAttribBindingARB::setLFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) const
{
    vao(binding)->bind();

    gl::glVertexAttribLFormat(attributeIndex(binding), size, type, relativeoffset);
}

} // namespace glo
