#include "VertexAttributeBindingImplementation_Legacy.h"

#include <cassert>

#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

struct Format
{
    enum Method
    {
        O = 0
        , I = 1
        , L = 2
    };

    Format()
    : method(O)
    , size(0)
    , type(static_cast<gl::GLenum>(0))
    , normalized(gl::GL_FALSE)
    , relativeoffset(0)
    {
    }

    Format(Method method, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset)
    : method(method)
    , size(size)
    , type(type)
    , normalized(normalized)
    , relativeoffset(relativeoffset)
    {
    }

    Method method;
    gl::GLint size;
    gl::GLenum type;
    gl::GLboolean normalized;
    gl::GLuint relativeoffset;
};

struct BindingData {
    Format m_format;
    gl::GLint m_baseoffset;
    gl::GLint m_stride;
    bool m_hasFormat;
    bool m_hasBuffer;
    bool m_hasAttribute;
};

namespace glo {

VertexAttributeBindingImplementation_Legacy::VertexAttributeBindingImplementation_Legacy()
{
}

VertexAttributeBindingImplementation_Legacy::~VertexAttributeBindingImplementation_Legacy()
{
}

BindingData * & VertexAttributeBindingImplementation_Legacy::bindingData(const VertexAttributeBinding * binding) const
{
    return reinterpret_cast<BindingData * &>(AbstractVertexAttributeBindingImplementation::bindingData(binding));
}

void VertexAttributeBindingImplementation_Legacy::bindAttribute(const VertexAttributeBinding * binding, gl::GLint /*attributeIndex*/) const
{
    if (bindingData(binding) == nullptr)
    {
        bindingData(binding) = new BindingData();
    }

    bindingData(binding)->m_hasAttribute = true;
    finishIfComplete(binding);
}

void VertexAttributeBindingImplementation_Legacy::bindBuffer(const VertexAttributeBinding * binding, const Buffer * /*vbo*/, gl::GLint baseoffset, gl::GLint stride) const
{
    if (bindingData(binding) == nullptr)
    {
        bindingData(binding) = new BindingData();
    }

    bindingData(binding)->m_baseoffset = baseoffset;
    bindingData(binding)->m_stride = stride;
    bindingData(binding)->m_hasBuffer = true;

    finishIfComplete(binding);
}

void VertexAttributeBindingImplementation_Legacy::setFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset) const
{
    if (bindingData(binding) == nullptr)
    {
        bindingData(binding) = new BindingData();
    }

    bindingData(binding)->m_format = Format(Format::O, size, type, normalized, relativeoffset);
    bindingData(binding)->m_hasFormat = true;

    finishIfComplete(binding);
}

void VertexAttributeBindingImplementation_Legacy::setIFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) const
{
    if (bindingData(binding) == nullptr)
    {
        bindingData(binding) = new BindingData();
    }

    bindingData(binding)->m_format = Format(Format::I, size, type, gl::GL_FALSE, relativeoffset);
    bindingData(binding)->m_hasFormat = true;

    finishIfComplete(binding);
}

void VertexAttributeBindingImplementation_Legacy::setLFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) const
{
    if (bindingData(binding) == nullptr)
    {
        bindingData(binding) = new BindingData();
    }

    bindingData(binding)->m_format = Format(Format::L, size, type, gl::GL_FALSE, relativeoffset);
    bindingData(binding)->m_hasFormat = true;

    finishIfComplete(binding);
}

void VertexAttributeBindingImplementation_Legacy::finishIfComplete(const VertexAttributeBinding * binding) const
{
    if (bindingData(binding) == nullptr)
    {
        bindingData(binding) = new BindingData();
    }

    if (bindingData(binding)->m_hasAttribute && bindingData(binding)->m_hasFormat && bindingData(binding)->m_hasBuffer)
    {
        finish(binding);
    }
}

void VertexAttributeBindingImplementation_Legacy::finish(const VertexAttributeBinding * binding) const
{
    assert(bindingData(binding) != nullptr);

    vao(binding)->bind();

    void * offset = nullptr;

    if (vbo(binding))
    {
        vbo(binding)->bind(gl::GL_ARRAY_BUFFER);
        offset = reinterpret_cast<void*>(bindingData(binding)->m_baseoffset + bindingData(binding)->m_format.relativeoffset);
    }
    else
    {
        Buffer::unbind(gl::GL_ARRAY_BUFFER);
    }

    const gl::GLint attribute = attributeIndex(binding);

    switch (bindingData(binding)->m_format.method)
    {
    case Format::I:
        gl::glVertexAttribIPointer(attribute, bindingData(binding)->m_format.size, bindingData(binding)->m_format.type, bindingData(binding)->m_stride, offset);

        break;
    case Format::L:
        gl::glVertexAttribLPointer(attribute, bindingData(binding)->m_format.size, bindingData(binding)->m_format.type, bindingData(binding)->m_stride, offset);

        break;
    default:
        gl::glVertexAttribPointer(attribute, bindingData(binding)->m_format.size, bindingData(binding)->m_format.type, bindingData(binding)->m_format.normalized, bindingData(binding)->m_stride, offset);
    }
}

} // namespace glo
