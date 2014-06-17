#pragma once



#include <vector>
#include <array>
#include <string>


#include <glowbase/ref_ptr.h>
#include <glowbase/Referenced.h>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/Program.h>

#include <glowutils/glowutils_api.h>

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
        AttributeFormat(gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeOffset, FormatType formatType);

        void setTo(glow::VertexAttributeBinding * binding) const;
    protected:
        gl::GLint size;
        gl::GLenum type;
        gl::GLboolean normalized;
        gl::GLuint relativeOffset;

        FormatType formatType;
    };

    VertexDrawable(gl::GLenum primitiveMode = gl::GL_TRIANGLES);
    VertexDrawable(gl::GLint baseOffset, gl::GLint stride, gl::GLenum primitiveMode = gl::GL_TRIANGLES);
    VertexDrawable(glow::Buffer* vbo, gl::GLint baseOffset, gl::GLint stride, gl::GLint size, gl::GLenum primitiveMode = gl::GL_TRIANGLES);
    template <typename T>
    VertexDrawable(const std::vector<T> & vertices, gl::GLenum primitiveMode = gl::GL_TRIANGLES);
    template <typename T, std::size_t Count>
    VertexDrawable(const std::array<T, Count> & vertices, gl::GLenum primitiveMode = gl::GL_TRIANGLES);

    void setBuffer(glow::Buffer* vbo, gl::GLint size);
    void setBuffer(glow::Buffer* vbo, gl::GLint baseOffset, gl::GLint stride, gl::GLint size);
    void setPrimitiveMode(gl::GLenum primitiveMode);

    template <typename T>
    void setVertices(const std::vector<T> & vertices);

    void setFormats(const std::vector<AttributeFormat> & formats);
    void bindAttributes(const std::vector<gl::GLint> & attributeIndices);

    void enableAll();

    void draw() const;
protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_vbo;
    std::vector<gl::GLint> m_attributeIndices;
    std::vector<AttributeFormat> m_formats;
    gl::GLint m_baseOffset;
    gl::GLint m_stride;
    gl::GLint m_size;
    gl::GLenum m_primitiveMode;
};

GLOWUTILS_API VertexDrawable::AttributeFormat Format(gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset, gl::GLboolean normalized = gl::GL_FALSE);
GLOWUTILS_API VertexDrawable::AttributeFormat FormatI(gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset);
GLOWUTILS_API VertexDrawable::AttributeFormat FormatL(gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset);

template <typename T>
VertexDrawable::VertexDrawable(const std::vector<T> & vertices, gl::GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_vbo(new glow::Buffer)
, m_baseOffset(0)
, m_stride(sizeof(T))
, m_size(static_cast<gl::GLint>(vertices.size()))
, m_primitiveMode(primitiveMode)
{
    m_vbo->setData(vertices);
}

template <typename T, std::size_t Count>
VertexDrawable::VertexDrawable(const std::array<T, Count> & vertices, gl::GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_vbo(new glow::Buffer)
, m_baseOffset(0)
, m_stride(sizeof(T))
, m_size(static_cast<gl::GLint>(vertices.size()))
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
