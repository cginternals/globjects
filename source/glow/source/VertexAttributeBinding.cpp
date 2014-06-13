#include <glow/VertexAttributeBinding.h>

#include <cassert>

#include <glowbase/Version.h>

#include <glow/VertexArrayObject.h>
#include <glow/glow.h>

#include "VertexAttributeBindingImplementation.h"

namespace glow
{

VertexAttributeBinding::VertexAttributeBinding(
    VertexArrayObject * vao
,   const gl::GLint bindingIndex)
: m_vao(vao)
, m_bindingIndex(bindingIndex)
, m_attributeIndex(0)
, m_vbo(nullptr)
, m_implementation(nullptr)
{
    assert(vao != nullptr);

    m_implementation = hasExtension(gl::Extension::ARB_vertex_attrib_binding)
        ? static_cast<VertexAttributeBindingImplementation*>(new VertexAttributeBinding_GL_4_3(this))
        : static_cast<VertexAttributeBindingImplementation*>(new VertexAttributeBinding_GL_3_0(this));

	setAttribute(bindingIndex); // as default
}

VertexAttributeBinding::~VertexAttributeBinding()
{
    delete m_implementation;
}

const VertexArrayObject * VertexAttributeBinding::vao() const
{
    return m_vao;
}

VertexArrayObject * VertexAttributeBinding::vao()
{
    return m_vao;
}

void VertexAttributeBinding::setAttribute(gl::GLint attributeIndex)
{
    m_attributeIndex = attributeIndex;
    m_implementation->bindAttribute(attributeIndex);
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
    //assert(vbo != nullptr);

    m_vbo = vbo;
    m_implementation->bindBuffer(vbo, baseoffset, stride);
}

void VertexAttributeBinding::setFormat(gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset)
{
    m_implementation->setFormat(size, type, normalized, relativeoffset);
}

void VertexAttributeBinding::setIFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset)
{
    m_implementation->setIFormat(size, type, relativeoffset);
}

void VertexAttributeBinding::setLFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset)
{
    m_implementation->setLFormat(size, type, relativeoffset);
}

} // namespace glow
