#include "VertexAttributeBindingImplementation.h"

#include <cassert>

#include <glbinding/functions.h>

#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>

namespace glow {

VertexAttributeBindingImplementation::VertexAttributeBindingImplementation(const VertexAttributeBinding* binding)
: m_binding(binding)
{
    assert(binding != nullptr);
}

VertexAttributeBindingImplementation::~VertexAttributeBindingImplementation()
{
}

gl::GLint VertexAttributeBindingImplementation::attributeIndex() const
{
    return m_binding->m_attributeIndex;
}

gl::GLint VertexAttributeBindingImplementation::bindingIndex() const
{
    return m_binding->m_bindingIndex;
}

const VertexArrayObject* VertexAttributeBindingImplementation::vao() const
{
    return m_binding->m_vao;
}

const Buffer* VertexAttributeBindingImplementation::vbo() const
{
    return m_binding->m_vbo;
}


VertexAttributeBinding_GL_3_0::VertexAttributeBinding_GL_3_0(const VertexAttributeBinding* binding)
: VertexAttributeBindingImplementation(binding)
, m_baseoffset(0)
, m_stride(0)
, m_hasFormat(false)
, m_hasBuffer(false)
, m_hasAttribute(false)
{
}

void VertexAttributeBinding_GL_3_0::bindAttribute(gl::GLint /*attributeIndex*/)
{
    m_hasAttribute = true;
    finishIfComplete();
}

void VertexAttributeBinding_GL_3_0::bindBuffer(const Buffer* /*vbo*/, gl::GLint baseoffset, gl::GLint stride)
{
    m_baseoffset = baseoffset;
    m_stride = stride;

    m_hasBuffer = true;
    finishIfComplete();
}

void VertexAttributeBinding_GL_3_0::setFormat(gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset)
{
    m_format = Format(Format::O, size, type, normalized, relativeoffset);

    m_hasFormat = true;
    finishIfComplete();
}

void VertexAttributeBinding_GL_3_0::setIFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset)
{
    m_format = Format(Format::I, size, type, gl::FALSE_, relativeoffset);

    m_hasFormat = true;
    finishIfComplete();
}

void VertexAttributeBinding_GL_3_0::setLFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset)
{
    m_format = Format(Format::L, size, type, gl::FALSE_, relativeoffset);

    m_hasFormat = true;
    finishIfComplete();
}

void VertexAttributeBinding_GL_3_0::finishIfComplete()
{
    if (m_hasAttribute && m_hasFormat && m_hasBuffer)
    {
        finish();
    }
}

void VertexAttributeBinding_GL_3_0::finish()
{
    vao()->bind();
    void * offset = nullptr;
    
    if (vbo())
    {
        vbo()->bind(gl::ARRAY_BUFFER);
        offset = reinterpret_cast<void*>(m_baseoffset + m_format.relativeoffset);
    }
    else
    {
        Buffer::unbind(gl::ARRAY_BUFFER);
    }

    const gl::GLint attribute = attributeIndex();

    switch (m_format.method)
    {
    case Format::I:
        gl::VertexAttribIPointer(attribute, m_format.size, m_format.type, m_stride, offset);

        break;
    case Format::L:
        gl::VertexAttribLPointer(attribute, m_format.size, m_format.type, m_stride, offset);

        break;
    default:
        gl::VertexAttribPointer(attribute, m_format.size, m_format.type, m_format.normalized, m_stride, offset);
    }
}

VertexAttributeBinding_GL_3_0::Format::Format()
: method(O)
, size(0)
, type(static_cast<gl::GLenum>(0))
, normalized(gl::FALSE_)
, relativeoffset(0)
{
}

VertexAttributeBinding_GL_3_0::Format::Format(Method method, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset)
: method(method)
, size(size)
, type(type)
, normalized(normalized)
, relativeoffset(relativeoffset)
{
}


VertexAttributeBinding_GL_4_3::VertexAttributeBinding_GL_4_3(const VertexAttributeBinding* binding)
: VertexAttributeBindingImplementation(binding)
{
}

void VertexAttributeBinding_GL_4_3::bindAttribute(gl::GLint attributeIndex)
{
    vao()->bind();

    gl::VertexAttribBinding(attributeIndex, bindingIndex());
}

void VertexAttributeBinding_GL_4_3::bindBuffer(const Buffer* vbo, gl::GLint baseoffset, gl::GLint stride)
{
    vao()->bind();

    gl::BindVertexBuffer(bindingIndex(), vbo ? vbo->id() : 0, baseoffset, stride);
}

void VertexAttributeBinding_GL_4_3::setFormat(gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset)
{
    vao()->bind();

    gl::VertexAttribFormat(attributeIndex(), size, type, normalized, relativeoffset);
}

void VertexAttributeBinding_GL_4_3::setIFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset)
{
    vao()->bind();

    gl::VertexAttribIFormat(attributeIndex(), size, type, relativeoffset);
}

void VertexAttributeBinding_GL_4_3::setLFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset)
{
    vao()->bind();

    gl::VertexAttribLFormat(attributeIndex(), size, type, relativeoffset);
}

} // namespace glow
