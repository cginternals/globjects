
#include "VertexAttributeBindingImplementation_DirectStateAccessARB.h"

#include <cassert>

#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>


namespace glo 
{

VertexAttributeBindingImplementation_DirectStateAccessARB::VertexAttributeBindingImplementation_DirectStateAccessARB()
{

}

VertexAttributeBindingImplementation_DirectStateAccessARB::~VertexAttributeBindingImplementation_DirectStateAccessARB()
{

}

void VertexAttributeBindingImplementation_DirectStateAccessARB::enable(const VertexArray * vertexArray, gl::GLint attributeIndex) const
{
    gl::glEnableVertexArrayAttrib(vertexArray->id(), attributeIndex);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::disable(const VertexArray * vertexArray, gl::GLint attributeIndex) const
{
    gl::glDisableVertexArrayAttrib(vertexArray->id(), attributeIndex);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::setAttributeDivisor(const VertexAttributeBinding * binding, gl::GLuint divisor) const
{
    gl::glVertexArrayBindingDivisor(vao(binding)->id(), bindingIndex(binding), divisor);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::bindAttribute(const VertexAttributeBinding * binding, gl::GLint attributeIndex) const
{
    gl::glVertexArrayAttribBinding(vao(binding)->id(), attributeIndex, bindingIndex(binding));
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::bindBuffer(const VertexAttributeBinding * binding, const Buffer * vbo, gl::GLint baseoffset, gl::GLint stride) const
{
    gl::glVertexArrayVertexBuffer(vao(binding)->id(), bindingIndex(binding), vbo ? vbo->id() : 0, baseoffset, stride);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::setFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset) const
{
    gl::glVertexArrayAttribFormat(vao(binding)->id(), attributeIndex(binding), size, type, normalized, relativeoffset);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::setIFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) const
{
    gl::glVertexArrayAttribIFormat(vao(binding)->id(), attributeIndex(binding), size, type, relativeoffset);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::setLFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) const
{
    gl::glVertexArrayAttribLFormat(vao(binding)->id(), attributeIndex(binding), size, type, relativeoffset);
}

} // namespace glo
