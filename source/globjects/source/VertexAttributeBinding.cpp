#include <globjects/VertexAttributeBinding.h>

#include <cassert>

#include <globjects/VertexArray.h>
#include <globjects/globjects.h>

#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractVertexAttributeBindingImplementation.h"


using namespace gl;

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
,   const GLint bindingIndex)
: m_vao(vao)
, m_bindingIndex(bindingIndex)
, m_attributeIndex(0)
, m_vbo(nullptr)
, m_bindingData(nullptr)
{
    assert(vao != nullptr);

	// setAttribute(bindingIndex); // as default - this leads to errors when binding unknown type 
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

void VertexAttributeBinding::setDivisor(GLint divisor)
{
    attributeImplementation().setAttributeDivisor(this, divisor);
}

void VertexAttributeBinding::setAttribute(GLint attributeIndex)
{
    m_attributeIndex = attributeIndex;
    attributeImplementation().bindAttribute(this, attributeIndex);
}

GLint VertexAttributeBinding::attributeIndex() const
{
    return m_attributeIndex;
}

GLint VertexAttributeBinding::bindingIndex() const
{
    return m_bindingIndex;
}

const Buffer * VertexAttributeBinding::buffer() const
{
    return m_vbo;
}

void VertexAttributeBinding::setBuffer(const Buffer * vbo, const GLint baseoffset, const GLint stride)
{
    m_vbo = vbo;
    attributeImplementation().bindBuffer(this, vbo, baseoffset, stride);
}

void VertexAttributeBinding::setFormat(const GLint size, const GLenum type, const GLboolean normalized, const GLuint relativeoffset)
{
    attributeImplementation().setFormat(this, size, type, normalized, relativeoffset);
}

void VertexAttributeBinding::setIFormat(const GLint size, const GLenum type, const GLuint relativeoffset)
{
    attributeImplementation().setIFormat(this, size, type, relativeoffset);
}

void VertexAttributeBinding::setLFormat(const GLint size, const GLenum type, const GLuint relativeoffset)
{
    attributeImplementation().setLFormat(this, size, type, relativeoffset);
}

} // namespace globjects
