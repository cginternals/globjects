
#include "VertexAttributeBindingImplementation_DirectStateAccessARB.h"

#include <cassert>

#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>


using namespace gl;

namespace globjects 
{

VertexAttributeBindingImplementation_DirectStateAccessARB::VertexAttributeBindingImplementation_DirectStateAccessARB()
{

}

VertexAttributeBindingImplementation_DirectStateAccessARB::~VertexAttributeBindingImplementation_DirectStateAccessARB()
{

}

void VertexAttributeBindingImplementation_DirectStateAccessARB::enable(const VertexArray * vertexArray, GLint attributeIndex) const
{
    glEnableVertexArrayAttrib(vertexArray->id(), attributeIndex);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::disable(const VertexArray * vertexArray, GLint attributeIndex) const
{
    glDisableVertexArrayAttrib(vertexArray->id(), attributeIndex);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::setAttributeDivisor(const VertexAttributeBinding * binding, GLuint divisor) const
{
    glVertexArrayBindingDivisor(vao(binding)->id(), bindingIndex(binding), divisor);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::bindAttribute(const VertexAttributeBinding * binding, GLint attributeIndex) const
{
    glVertexArrayAttribBinding(vao(binding)->id(), attributeIndex, bindingIndex(binding));
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::bindBuffer(const VertexAttributeBinding * binding, const Buffer * vbo, GLint baseoffset, GLint stride) const
{
    glVertexArrayVertexBuffer(vao(binding)->id(), bindingIndex(binding), vbo ? vbo->id() : 0, baseoffset, stride);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::setFormat(const VertexAttributeBinding * binding, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) const
{
    glVertexArrayAttribFormat(vao(binding)->id(), attributeIndex(binding), size, type, normalized, relativeoffset);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::setIFormat(const VertexAttributeBinding * binding, GLint size, GLenum type, GLuint relativeoffset) const
{
    glVertexArrayAttribIFormat(vao(binding)->id(), attributeIndex(binding), size, type, relativeoffset);
}

void VertexAttributeBindingImplementation_DirectStateAccessARB::setLFormat(const VertexAttributeBinding * binding, GLint size, GLenum type, GLuint relativeoffset) const
{
    glVertexArrayAttribLFormat(vao(binding)->id(), attributeIndex(binding), size, type, relativeoffset);
}

} // namespace globjects
