#include <glowutils/VertexDrawable.h>

#include <glow/VertexAttributeBinding.h>

namespace glowutils
{

VertexDrawable::AttributeFormat::AttributeFormat()
: size(0)
, type(GL_FLOAT)
, normalized(GL_FALSE)
, relativeOffset(0)
, formatType(Float)
{
}

VertexDrawable::AttributeFormat::AttributeFormat(GLint size, GLenum type, GLboolean normalized, GLuint relativeOffset, FormatType formatType)
: size(size)
, type(type)
, normalized(normalized)
, relativeOffset(relativeOffset)
, formatType(formatType)
{
}

void VertexDrawable::AttributeFormat::setTo(glow::VertexAttributeBinding * binding) const
{
    switch (formatType)
    {
        case Float:
            binding->setFormat(size, type, normalized, relativeOffset);
            break;
        case Integer:
            binding->setIFormat(size, type, relativeOffset);
            break;
        case Long:
            binding->setLFormat(size, type, relativeOffset);
            break;
    }
}

VertexDrawable::VertexDrawable(GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_baseOffset(0)
, m_stride(0)
, m_size(0)
, m_primitiveMode(primitiveMode)
{
}

VertexDrawable::VertexDrawable(GLint baseOffset, GLint stride, GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_baseOffset(baseOffset)
, m_stride(stride)
, m_size(0)
, m_primitiveMode(primitiveMode)
{
}

VertexDrawable::VertexDrawable(glow::Buffer* vbo, GLint baseOffset, GLint stride, GLint size, GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_vbo(vbo)
, m_baseOffset(baseOffset)
, m_stride(stride)
, m_size(size)
, m_primitiveMode(primitiveMode)
{
}

void VertexDrawable::setBuffer(glow::Buffer* vbo, GLint size)
{
    m_size = size;
    m_vbo = vbo;

    for (unsigned i = 0; i<m_formats.size(); ++i)
    {
        m_vao->binding(i)->setBuffer(m_vbo, m_baseOffset, m_stride);
    }
}

void VertexDrawable::setBuffer(glow::Buffer* vbo, GLint baseOffset, GLint stride, GLint size)
{
    m_baseOffset = baseOffset;
    m_stride = stride;
    setBuffer(vbo, size);
}

void VertexDrawable::setPrimitiveMode(GLenum primitiveMode)
{
    m_primitiveMode = primitiveMode;
}

void VertexDrawable::setFormats(const std::vector<AttributeFormat> & formats)
{
    m_formats = formats;

    m_attributeIndices.clear();

    for (unsigned i = 0; i<m_formats.size(); ++i)
    {
        auto binding = m_vao->binding(i);
        binding->setAttribute(i);
        binding->setBuffer(m_vbo, m_baseOffset, m_stride);
        formats[i].setTo(binding);

        m_attributeIndices.push_back(i);
    }
}

void VertexDrawable::bindAttributes(const std::vector<GLint> & attributeIndices)
{
    m_attributeIndices = attributeIndices;

    for (unsigned i = 0; i<m_attributeIndices.size(); ++i)
    {
        GLint index = m_attributeIndices[i];
        if (index>=0)
            m_vao->binding(i)->setAttribute(index);
    }
}

void VertexDrawable::enableAll()
{
    for (int index : m_attributeIndices)
    {
        if (index>=0)
            m_vao->enable(index);
    }
}

void VertexDrawable::draw() const
{
    if (!m_vbo || m_size <= 0)
        return;

    m_vao->drawArrays(m_primitiveMode, 0, m_size);
}

VertexDrawable::AttributeFormat Format(GLint size, GLenum type, GLuint relativeOffset, GLboolean normalized)
{
    return VertexDrawable::AttributeFormat(size, type, normalized, relativeOffset, VertexDrawable::AttributeFormat::Float);
}

VertexDrawable::AttributeFormat FormatI(GLint size, GLenum type, GLuint relativeOffset)
{
    return VertexDrawable::AttributeFormat(size, type, GL_FALSE, relativeOffset, VertexDrawable::AttributeFormat::Integer);
}

VertexDrawable::AttributeFormat FormatL(GLint size, GLenum type, GLuint relativeOffset)
{
    return VertexDrawable::AttributeFormat(size, type, GL_FALSE, relativeOffset, VertexDrawable::AttributeFormat::Long);
}

} // namespace glowutils
