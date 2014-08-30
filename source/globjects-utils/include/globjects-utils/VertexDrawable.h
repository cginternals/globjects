#pragma once

#include <vector>
#include <array>
#include <string>

#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects-base/ref_ptr.h>
#include <globjects-base/Referenced.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>

#include <globjects-utils/globjects-utils_api.h>

namespace gloutils
{

class GLOBJECTS_UTILS_API VertexDrawable : public glo::Referenced
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

        void setTo(glo::VertexAttributeBinding * binding) const;
    protected:
        gl::GLint size;
        gl::GLenum type;
        gl::GLboolean normalized;
        gl::GLuint relativeOffset;

        FormatType formatType;
    };

    VertexDrawable(gl::GLenum primitiveMode = gl::GL_TRIANGLES);
    VertexDrawable(gl::GLint baseOffset, gl::GLint stride, gl::GLenum primitiveMode = gl::GL_TRIANGLES);
    VertexDrawable(glo::Buffer* vbo, gl::GLint baseOffset, gl::GLint stride, gl::GLint size, gl::GLenum primitiveMode = gl::GL_TRIANGLES);
    template <typename T>
    VertexDrawable(const std::vector<T> & vertices, gl::GLenum primitiveMode = gl::GL_TRIANGLES);
    template <typename T, std::size_t Count>
    VertexDrawable(const std::array<T, Count> & vertices, gl::GLenum primitiveMode = gl::GL_TRIANGLES);

    void setBuffer(glo::Buffer* vbo, gl::GLint size);
    void setBuffer(glo::Buffer* vbo, gl::GLint baseOffset, gl::GLint stride, gl::GLint size);
    void setPrimitiveMode(gl::GLenum primitiveMode);

    template <typename T>
    void setVertices(const std::vector<T> & vertices);

    void setFormats(const std::vector<AttributeFormat> & formats);
    void bindAttributes(const std::vector<gl::GLint> & attributeIndices);

    void enableAll();

    void draw() const;
protected:
    glo::ref_ptr<glo::VertexArray> m_vao;
    glo::ref_ptr<glo::Buffer> m_vbo;
    std::vector<gl::GLint> m_attributeIndices;
    std::vector<AttributeFormat> m_formats;
    gl::GLint m_baseOffset;
    gl::GLint m_stride;
    gl::GLint m_size;
    gl::GLenum m_primitiveMode;
};

GLOBJECTS_UTILS_API VertexDrawable::AttributeFormat Format(gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset, gl::GLboolean normalized = gl::GL_FALSE);
GLOBJECTS_UTILS_API VertexDrawable::AttributeFormat FormatI(gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset);
GLOBJECTS_UTILS_API VertexDrawable::AttributeFormat FormatL(gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset);

template <typename T>
VertexDrawable::VertexDrawable(const std::vector<T> & vertices, gl::GLenum primitiveMode)
: m_vao(new glo::VertexArray)
, m_vbo(new glo::Buffer)
, m_baseOffset(0)
, m_stride(sizeof(T))
, m_size(static_cast<gl::GLint>(vertices.size()))
, m_primitiveMode(primitiveMode)
{
    m_vbo->setData(vertices);
}

template <typename T, std::size_t Count>
VertexDrawable::VertexDrawable(const std::array<T, Count> & vertices, gl::GLenum primitiveMode)
: m_vao(new glo::VertexArray)
, m_vbo(new glo::Buffer)
, m_baseOffset(0)
, m_stride(sizeof(T))
, m_size(static_cast<gl::GLint>(vertices.size()))
, m_primitiveMode(primitiveMode)
{
    m_vbo->setData(vertices, gl::GL_STATIC_DRAW);
}

template <typename T>
void VertexDrawable::setVertices(const std::vector<T> & vertices)
{
    m_vbo->setData(vertices, gl::GL_STATIC_DRAW);
}

} // namespace gloutils
