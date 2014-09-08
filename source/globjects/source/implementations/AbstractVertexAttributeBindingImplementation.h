#pragma once

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

#include <globjects/VertexArray.h>

namespace globjects {

class VertexArray;
class VertexAttributeBinding;
class Buffer;

class AbstractVertexAttributeBindingImplementation
{
public:
    AbstractVertexAttributeBindingImplementation();
    virtual ~AbstractVertexAttributeBindingImplementation();

    static AbstractVertexAttributeBindingImplementation * get(VertexArray::AttributeImplementation impl =
        VertexArray::AttributeImplementation::VertexAttribBindingARB);

    virtual void enable(const VertexArray * vertexArray, gl::GLint attributeIndex) const = 0;
    virtual void disable(const VertexArray * vertexArray, gl::GLint attributeIndex) const = 0;

    virtual void setAttributeDivisor(const VertexAttributeBinding * binding, gl::GLuint divisor) const = 0;

    virtual void bindAttribute(const VertexAttributeBinding * binding, gl::GLint attributeIndex) const = 0;
    virtual void bindBuffer(const VertexAttributeBinding * binding, const Buffer * vbo, gl::GLint baseoffset, gl::GLint stride) const = 0;

    virtual void setFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset) const = 0;
    virtual void setIFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) const = 0;
    virtual void setLFormat(const VertexAttributeBinding * binding, gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) const = 0;
protected:
    gl::GLint attributeIndex(const VertexAttributeBinding * binding) const;
    gl::GLint bindingIndex(const VertexAttributeBinding * binding) const;

    const VertexArray * vao(const VertexAttributeBinding * binding) const;
    const Buffer * vbo(const VertexAttributeBinding * binding) const;

    void * & bindingData(const VertexAttributeBinding * binding) const;
};

} // namespace globjects
