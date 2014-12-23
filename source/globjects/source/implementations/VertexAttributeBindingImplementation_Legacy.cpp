#include "VertexAttributeBindingImplementation_Legacy.h"

#include <cassert>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>


using namespace gl;

namespace globjects 
{

VertexAttributeBindingImplementation_Legacy::Format::Format()
: method(Method::O)
, size(0)
, type(GL_INVALID_ENUM)
, normalized(GL_FALSE)
, relativeoffset(0)
{
}

VertexAttributeBindingImplementation_Legacy::Format::Format(
    VertexAttributeBindingImplementation_Legacy::Format::Method method, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
: method(method)
, size(size)
, type(type)
, normalized(normalized)
, relativeoffset(relativeoffset)
{
}

VertexAttributeBindingImplementation_Legacy::BindingData::BindingData()
: baseoffset(0)
, stride(0)
, hasFormat(false)
, hasBuffer(false)
, hasAttribute(false)
{
}

VertexAttributeBindingImplementation_Legacy::VertexAttributeBindingImplementation_Legacy()
{
}

VertexAttributeBindingImplementation_Legacy::~VertexAttributeBindingImplementation_Legacy()
{
}

void VertexAttributeBindingImplementation_Legacy::enable(const VertexArray * vertexArray, GLint attributeIndex) const
{
    vertexArray->bind();

    glEnableVertexAttribArray(attributeIndex);
}

void VertexAttributeBindingImplementation_Legacy::disable(const VertexArray * vertexArray, GLint attributeIndex) const
{
    vertexArray->bind();

    glDisableVertexAttribArray(attributeIndex);
}

void VertexAttributeBindingImplementation_Legacy::setAttributeDivisor(const VertexAttributeBinding * binding, GLuint divisor) const
{
    vao(binding)->bind();

    glVertexAttribDivisor(binding->attributeIndex(), divisor);
}

VertexAttributeBindingImplementation_Legacy::BindingData * & VertexAttributeBindingImplementation_Legacy::bindingData(const VertexAttributeBinding * binding) const
{
    return reinterpret_cast<BindingData * &>(AbstractVertexAttributeBindingImplementation::bindingData(binding));
}

void VertexAttributeBindingImplementation_Legacy::bindAttribute(const VertexAttributeBinding * binding, GLint /*attributeIndex*/) const
{
    if (bindingData(binding) == nullptr)
        bindingData(binding) = new BindingData();

    bindingData(binding)->hasAttribute = true;
    finishIfComplete(binding);
}

void VertexAttributeBindingImplementation_Legacy::bindBuffer(const VertexAttributeBinding * binding, const Buffer * /*vbo*/, GLint baseoffset, GLint stride) const
{
    if (bindingData(binding) == nullptr)
        bindingData(binding) = new BindingData();

    bindingData(binding)->baseoffset = baseoffset;
    bindingData(binding)->stride = stride;
    bindingData(binding)->hasBuffer = true;

    finishIfComplete(binding);
}

void VertexAttributeBindingImplementation_Legacy::setFormat(const VertexAttributeBinding * binding, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) const
{
    if (bindingData(binding) == nullptr)
        bindingData(binding) = new BindingData();

    bindingData(binding)->format = Format(Format::Method::O, size, type, normalized, relativeoffset);
    bindingData(binding)->hasFormat = true;

    finishIfComplete(binding);
}

void VertexAttributeBindingImplementation_Legacy::setIFormat(const VertexAttributeBinding * binding, GLint size, GLenum type, GLuint relativeoffset) const
{
    if (bindingData(binding) == nullptr)
        bindingData(binding) = new BindingData();

    bindingData(binding)->format = Format(Format::Method::I, size, type, GL_FALSE, relativeoffset);
    bindingData(binding)->hasFormat = true;

    finishIfComplete(binding);
}

void VertexAttributeBindingImplementation_Legacy::setLFormat(const VertexAttributeBinding * binding, GLint size, GLenum type, GLuint relativeoffset) const
{
    if (bindingData(binding) == nullptr)
        bindingData(binding) = new BindingData();

    bindingData(binding)->format = Format(Format::Method::L, size, type, GL_FALSE, relativeoffset);
    bindingData(binding)->hasFormat = true;

    finishIfComplete(binding);
}

void VertexAttributeBindingImplementation_Legacy::finishIfComplete(const VertexAttributeBinding * binding) const
{
    if (bindingData(binding) == nullptr)
        bindingData(binding) = new BindingData();

    if (bindingData(binding)->hasAttribute && bindingData(binding)->hasFormat && bindingData(binding)->hasBuffer)
        finish(binding);
}

void VertexAttributeBindingImplementation_Legacy::finish(const VertexAttributeBinding * binding) const
{
    assert(bindingData(binding) != nullptr);

    vao(binding)->bind();

    void * offset = nullptr;

    if (vbo(binding))
    {
        vbo(binding)->bind(GL_ARRAY_BUFFER);
        const auto offset64 = static_cast<GLuint64>(bindingData(binding)->baseoffset + bindingData(binding)->format.relativeoffset);
        offset = reinterpret_cast<void *>(offset64);
    }
    else
    {
        Buffer::unbind(GL_ARRAY_BUFFER);
    }

    const GLint attribute = attributeIndex(binding);

    switch (bindingData(binding)->format.method)
    {
    case Format::Method::I:
        glVertexAttribIPointer(attribute, bindingData(binding)->format.size, bindingData(binding)->format.type
            , bindingData(binding)->stride, offset);
        break;

    case Format::Method::L:
        glVertexAttribLPointer(attribute, bindingData(binding)->format.size, bindingData(binding)->format.type
            , bindingData(binding)->stride, offset);
        break;

    default:
        glVertexAttribPointer(attribute, bindingData(binding)->format.size, bindingData(binding)->format.type
            , bindingData(binding)->format.normalized, bindingData(binding)->stride, offset);
    }
}

} // namespace globjects
