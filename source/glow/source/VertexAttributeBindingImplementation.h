#pragma once

#include <GL/glew.h>

#include <glow/glow.h>

namespace glow {

class VertexAttributeBinding;
class Buffer;
class VertexArrayObject;

class VertexAttributeBindingImplementation
{
public:
    VertexAttributeBindingImplementation(const VertexAttributeBinding * binding);
    virtual ~VertexAttributeBindingImplementation();

    GLint attributeIndex() const;
    GLint bindingIndex() const;

    const VertexArrayObject * vao() const;
    const Buffer * vbo() const;

    virtual void bindAttribute(GLint attributeIndex) = 0;
    virtual void bindBuffer(
        const Buffer * vbo
    ,   GLint baseoffset
    ,   GLint stride) = 0;

    virtual void setFormat(
        GLint size
    ,   GLenum type
    ,   GLboolean normalized
    ,   GLuint relativeoffset) = 0;

    virtual void setIFormat(GLint size, GLenum type, GLuint relativeoffset) = 0;
    virtual void setLFormat(GLint size, GLenum type, GLuint relativeoffset) = 0;

protected:
    const VertexAttributeBinding * m_binding;
};


class VertexAttributeBinding_GL_3_0 : public VertexAttributeBindingImplementation
{
public:
    VertexAttributeBinding_GL_3_0(const VertexAttributeBinding * binding);

    virtual void bindAttribute(GLint attributeIndex);
    virtual void bindBuffer(const Buffer* vbo, GLint baseoffset, GLint stride);

    virtual void setFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
    virtual void setIFormat(GLint size, GLenum type, GLuint relativeoffset);
    virtual void setLFormat(GLint size, GLenum type, GLuint relativeoffset);

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
        ,   GLint size
        ,   GLenum type
        ,   GLboolean normalized
        ,   GLuint relativeoffset);

        Method method;

        GLint size;
        GLenum type;
        GLboolean normalized;
        GLuint relativeoffset;
    };

    Format m_format;
    GLint m_baseoffset;
    GLint m_stride;

    bool m_hasFormat;
    bool m_hasBuffer;
    bool m_hasAttribute;
};

class VertexAttributeBinding_GL_4_3 : public VertexAttributeBindingImplementation
{
public:
    VertexAttributeBinding_GL_4_3(const VertexAttributeBinding* binding);

    virtual void bindAttribute(GLint attributeIndex);
    virtual void bindBuffer(const Buffer * vbo, GLint baseoffset, GLint stride);

    virtual void setFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
    virtual void setIFormat(GLint size, GLenum type, GLuint relativeoffset);
    virtual void setLFormat(GLint size, GLenum type, GLuint relativeoffset);
};

} // namespace glow
