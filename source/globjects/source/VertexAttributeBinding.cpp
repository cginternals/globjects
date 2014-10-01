#include <globjects/VertexAttributeBinding.h>

#include <cassert>

#include <glbinding/Version.h>

#include <globjects/VertexArray.h>
#include <globjects/globjects.h>

#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractVertexAttributeBindingImplementation.h"

namespace 
{
    globjects::AbstractVertexAttributeBindingImplementation & attributeImplementation()
    {
        return globjects::ImplementationRegistry::current().attributeImplementation();
    }
}

namespace globjects
{

VertexAttributeBinding::VertexAttributeBinding(
    VertexArray * vao
,   const gl::GLint bindingIndex)
: m_vao(vao)
, m_bindingIndex(bindingIndex)
, m_attributeIndex(0)
, m_vbo(nullptr)
, m_bindingData(nullptr)
{
    assert(vao != nullptr);

	setAttribute(bindingIndex); // as default
}

VertexAttributeBinding::~VertexAttributeBinding()
{
}

const VertexArray * VertexAttributeBinding::vao() const
{
    return m_vao;
}

VertexArray * VertexAttributeBinding::vao()
{
    return m_vao;
}

void VertexAttributeBinding::setDivisor(gl::GLint divisor)
{
    attributeImplementation().setAttributeDivisor(this, divisor);
}

void VertexAttributeBinding::setAttribute(gl::GLint attributeIndex)
{
    m_attributeIndex = attributeIndex;
    attributeImplementation().bindAttribute(this, attributeIndex);
}

gl::GLint VertexAttributeBinding::attributeIndex() const
{
    return m_attributeIndex;
}

gl::GLint VertexAttributeBinding::bindingIndex() const
{
    return m_bindingIndex;
}

const Buffer* VertexAttributeBinding::buffer() const
{
    return m_vbo;
}

void VertexAttributeBinding::setBuffer(const Buffer* vbo, gl::GLint baseoffset, gl::GLint stride)
{
    m_vbo = vbo;
    attributeImplementation().bindBuffer(this, vbo, baseoffset, stride);
}

void VertexAttributeBinding::setFormat(gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset)
{
    attributeImplementation().setFormat(this, size, type, normalized, relativeoffset);
}

void VertexAttributeBinding::setIFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset)
{
    attributeImplementation().setIFormat(this, size, type, relativeoffset);
}

void VertexAttributeBinding::setLFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset)
{
    attributeImplementation().setLFormat(this, size, type, relativeoffset);
}

} // namespace globjects
