
#pragma once


#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

#include <globjects/VertexArray.h>


namespace globjects
{


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

    virtual gl::GLuint create() const = 0;
    virtual void destroy(gl::GLuint id) const = 0;

    virtual void bind(const VertexArray * vertexArray) const;
    virtual void unbind() const;

    virtual void drawArrays(const VertexArray * vertexArray, gl::GLenum mode, gl::GLint first, gl::GLsizei count) const;
    virtual void drawArraysInstanced(const VertexArray * vertexArray, gl::GLenum mode, gl::GLint first, gl::GLsizei count, gl::GLsizei instanceCount) const;
    virtual void drawArraysInstancedBaseInstance(const VertexArray * vertexArray, gl::GLenum mode, gl::GLint first, gl::GLsizei count, gl::GLsizei instanceCount, gl::GLuint baseInstance) const;
    virtual void drawArraysIndirect(const VertexArray * vertexArray, gl::GLenum mode, const void * indirect = nullptr) const;

    virtual void multiDrawArrays(const VertexArray * vertexArray, gl::GLenum mode, gl::GLint * first, const gl::GLsizei * count, gl::GLsizei drawCount) const;
    virtual void multiDrawArraysIndirect(const VertexArray * vertexArray, gl::GLenum mode, const void * indirect, gl::GLsizei drawCount, gl::GLsizei stride) const;

    virtual void drawElements(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices = nullptr) const;
    virtual void drawElementsBaseVertex(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLint baseVertex) const;
    virtual void drawElementsInstanced(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei primitiveCount) const;
    virtual void drawElementsInstancedBaseInstance(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei instanceCount, gl::GLuint baseInstance) const;
    virtual void drawElementsInstancedBaseVertex(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei instanceCount, gl::GLint baseVertex) const;
    virtual void drawElementsInstancedBaseVertexBaseInstance(const VertexArray * vertexArray, gl::GLenum mode, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLsizei instanceCount, gl::GLint baseVertex, gl::GLuint baseInstance) const;

    virtual void multiDrawElements(const VertexArray * vertexArray, gl::GLenum mode, const gl::GLsizei * count, gl::GLenum type, const void ** indices, gl::GLsizei drawCount) const;
    virtual void multiDrawElementsBaseVertex(const VertexArray * vertexArray, gl::GLenum mode, const gl::GLsizei * count, gl::GLenum type, const void ** indices, gl::GLsizei drawCount, gl::GLint * baseVertex) const;
    virtual void multiDrawElementsIndirect(const VertexArray * vertexArray, gl::GLenum mode, gl::GLenum type, const void * indirect, gl::GLsizei drawCount, gl::GLsizei stride) const;

    virtual void drawRangeElements(const VertexArray * vertexArray, gl::GLenum mode, gl::GLuint start, gl::GLuint end, gl::GLsizei count, gl::GLenum type, const void * indices = nullptr) const;
    virtual void drawRangeElementsBaseVertex(const VertexArray * vertexArray, gl::GLenum mode, gl::GLuint start, gl::GLuint end, gl::GLsizei count, gl::GLenum type, const void * indices, gl::GLint baseVertex) const;

    virtual void enable(const VertexArray * vertexArray, gl::GLint attributeIndex) const = 0;
    virtual void disable(const VertexArray * vertexArray, gl::GLint attributeIndex) const = 0;

    virtual void bindElementBuffer(const VertexArray * vertexArray, const Buffer * ebo) const = 0;

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
