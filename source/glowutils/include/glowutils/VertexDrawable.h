#pragma once

#include <GL/glew.h>

#include <vector>
#include <array>
#include <string>

#include <glowutils/glowutils.h>

#include <glow/ref_ptr.h>
#include <glow/Referenced.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/Program.h>

namespace glowutils
{

class GLOWUTILS_API VertexDrawable : public glow::Referenced
{
public:
    class AttributeFormat
    {
    public:
        enum FormatType
        {
            Float,
            Integer,
            Long
        };

        AttributeFormat();
        AttributeFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeOffset, FormatType formatType);

        void setTo(glow::VertexAttributeBinding * binding) const;
    protected:
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLuint relativeOffset;

        FormatType formatType;
    };

    VertexDrawable(GLenum primitiveMode = GL_TRIANGLES);
    VertexDrawable(GLint baseOffset, GLint stride, GLenum primitiveMode = GL_TRIANGLES);
    VertexDrawable(glow::Buffer* vbo, GLint baseOffset, GLint stride, GLint size, GLenum primitiveMode = GL_TRIANGLES);
    template <typename T>
    VertexDrawable(const std::vector<T> & vertices, GLenum primitiveMode = GL_TRIANGLES);
    template <typename T, std::size_t Count>
    VertexDrawable(const std::array<T, Count> & vertices, GLenum primitiveMode = GL_TRIANGLES);

    void setBuffer(glow::Buffer* vbo, GLint size);
    void setBuffer(glow::Buffer* vbo, GLint baseOffset, GLint stride, GLint size);
    void setPrimitiveMode(GLenum primitiveMode);

    template <typename T>
    void setVertices(const std::vector<T> & vertices);

    void setFormats(const std::vector<AttributeFormat> & formats);
    void bindAttributes(const std::vector<GLint> & attributeIndices);

    void enableAll();

    void draw();
protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_vbo;
    std::vector<GLint> m_attributeIndices;
    std::vector<AttributeFormat> m_formats;
    GLint m_baseOffset;
    GLint m_stride;
    GLint m_size;
    GLenum m_primitiveMode;
};

GLOWUTILS_API VertexDrawable::AttributeFormat Format(GLint size, GLenum type, GLuint relativeOffset, GLboolean normalized = GL_FALSE);
GLOWUTILS_API VertexDrawable::AttributeFormat FormatI(GLint size, GLenum type, GLuint relativeOffset);
GLOWUTILS_API VertexDrawable::AttributeFormat FormatL(GLint size, GLenum type, GLuint relativeOffset);

template <typename T>
VertexDrawable::VertexDrawable(const std::vector<T> & vertices, GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_vbo(new glow::Buffer(GL_ARRAY_BUFFER))
, m_baseOffset(0)
, m_stride(sizeof(T))
, m_size(static_cast<GLint>(vertices.size()))
, m_primitiveMode(primitiveMode)
{
    m_vbo->setData(vertices);
}

template <typename T, std::size_t Count>
VertexDrawable::VertexDrawable(const std::array<T, Count> & vertices, GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_vbo(new glow::Buffer(GL_ARRAY_BUFFER))
, m_baseOffset(0)
, m_stride(sizeof(T))
, m_size(static_cast<GLint>(vertices.size()))
, m_primitiveMode(primitiveMode)
{
    m_vbo->setData(vertices);
}

template <typename T>
void VertexDrawable::setVertices(const std::vector<T> & vertices)
{
    m_vbo->setData(vertices);
}

} // namespace glowutils
