#pragma once

#include <GL/glew.h>

#include <vector>
#include <array>

#include <glowutils/glowutils.h>

#include <glow/ref_ptr.h>
#include <glow/Referenced.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>

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

    VertexDrawable(glow::Buffer* vbo, GLint baseOffset, GLint stride, GLint size, GLenum primitiveMode = GL_TRIANGLES);
    template <typename T>
    VertexDrawable(const std::vector<T> & vertices, GLenum primitiveMode = GL_TRIANGLES);
    template <typename T, std::size_t Count>
    VertexDrawable(const std::array<T, Count> & vertices, GLenum primitiveMode = GL_TRIANGLES);

    void setFormats(const std::vector<AttributeFormat> & formats);
    void bindAttributes(const std::vector<GLint> & attributeIndices);

    void draw();
protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_vbo;
    GLint m_baseOffset;
    GLint m_stride;
    GLint m_size;
    GLenum m_primitiveMode;
};

GLOWUTILS_API VertexDrawable::AttributeFormat Format(GLint size, GLenum type, GLuint relativeOffset, GLboolean normalized = GL_FALSE)
{
    return VertexDrawable::AttributeFormat(size, type, normalized, relativeOffset, VertexDrawable::AttributeFormat::Float);
}

GLOWUTILS_API VertexDrawable::AttributeFormat FormatI(GLint size, GLenum type, GLuint relativeOffset)
{
    return VertexDrawable::AttributeFormat(size, type, GL_FALSE, relativeOffset, VertexDrawable::AttributeFormat::Integer);
}

GLOWUTILS_API VertexDrawable::AttributeFormat FormatL(GLint size, GLenum type, GLuint relativeOffset)
{
    return VertexDrawable::AttributeFormat(size, type, GL_FALSE, relativeOffset, VertexDrawable::AttributeFormat::Long);
}

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

} // namespace glowutils
