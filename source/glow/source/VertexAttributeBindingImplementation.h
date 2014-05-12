#pragma once

#include <glbinding/types.h>

#include <glow/glow_api.h>

namespace glow {

class VertexAttributeBinding;
class Buffer;
class VertexArrayObject;

class VertexAttributeBindingImplementation
{
public:
    VertexAttributeBindingImplementation(const VertexAttributeBinding * binding);
    virtual ~VertexAttributeBindingImplementation();

    gl::GLint attributeIndex() const;
    gl::GLint bindingIndex() const;

    const VertexArrayObject * vao() const;
    const Buffer * vbo() const;

    virtual void bindAttribute(gl::GLint attributeIndex) = 0;
    virtual void bindBuffer(
        const Buffer * vbo
    ,   gl::GLint baseoffset
    ,   gl::GLint stride) = 0;

    virtual void setFormat(
        gl::GLint size
    ,   gl::GLenum type
    ,   gl::GLboolean normalized
    ,   gl::GLuint relativeoffset) = 0;

    virtual void setIFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) = 0;
    virtual void setLFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset) = 0;

protected:
    const VertexAttributeBinding * m_binding;
};


class VertexAttributeBinding_GL_3_0 : public VertexAttributeBindingImplementation
{
public:
    VertexAttributeBinding_GL_3_0(const VertexAttributeBinding * binding);

    virtual void bindAttribute(gl::GLint attributeIndex);
    virtual void bindBuffer(const Buffer* vbo, gl::GLint baseoffset, gl::GLint stride);

    virtual void setFormat(gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset);
    virtual void setIFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset);
    virtual void setLFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset);

protected:
    void finishIfComplete();
    void finish();

protected:
    struct Format
    {
        enum Method
        {
            O = 0
        ,   I = 1
        ,   L = 2
        };

        Format();
        Format(
            Method method
        ,   gl::GLint size
        ,   gl::GLenum type
        ,   gl::GLboolean normalized
        ,   gl::GLuint relativeoffset);

        Method method;

        gl::GLint size;
        gl::GLenum type;
        gl::GLboolean normalized;
        gl::GLuint relativeoffset;
    };

    Format m_format;
    gl::GLint m_baseoffset;
    gl::GLint m_stride;

    bool m_hasFormat;
    bool m_hasBuffer;
    bool m_hasAttribute;
};

class VertexAttributeBinding_GL_4_3 : public VertexAttributeBindingImplementation
{
public:
    VertexAttributeBinding_GL_4_3(const VertexAttributeBinding* binding);

    virtual void bindAttribute(gl::GLint attributeIndex);
    virtual void bindBuffer(const Buffer * vbo, gl::GLint baseoffset, gl::GLint stride);

    virtual void setFormat(gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeoffset);
    virtual void setIFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset);
    virtual void setLFormat(gl::GLint size, gl::GLenum type, gl::GLuint relativeoffset);
};

} // namespace glow
