#include <glowutils/VertexDrawable.h>

#include <glow/VertexAttributeBinding.h>

namespace glowutils
{

VertexDrawable::AttributeFormat::AttributeFormat()
: size(0)
, type(gl::FLOAT)
, normalized(gl::FALSE_)
, relativeOffset(0)
, formatType(Float)
{
}

VertexDrawable::AttributeFormat::AttributeFormat(gl::GLint size, gl::GLenum type, gl::GLboolean normalized, gl::GLuint relativeOffset, FormatType formatType)
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

VertexDrawable::VertexDrawable(gl::GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_baseOffset(0)
, m_stride(0)
, m_size(0)
, m_primitiveMode(primitiveMode)
{
}

VertexDrawable::VertexDrawable(gl::GLint baseOffset, gl::GLint stride, gl::GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_baseOffset(baseOffset)
, m_stride(stride)
, m_size(0)
, m_primitiveMode(primitiveMode)
{
}

VertexDrawable::VertexDrawable(glow::Buffer* vbo, gl::GLint baseOffset, gl::GLint stride, gl::GLint size, gl::GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_vbo(vbo)
, m_baseOffset(baseOffset)
, m_stride(stride)
, m_size(size)
, m_primitiveMode(primitiveMode)
{
}

void VertexDrawable::setBuffer(glow::Buffer* vbo, gl::GLint size)
{
    m_size = size;
    m_vbo = vbo;

    for (unsigned i = 0; i<m_formats.size(); ++i)
    {
        m_vao->binding(i)->setBuffer(m_vbo, m_baseOffset, m_stride);
    }
}

void VertexDrawable::setBuffer(glow::Buffer* vbo, gl::GLint baseOffset, gl::GLint stride, gl::GLint size)
{
    m_baseOffset = baseOffset;
    m_stride = stride;
    setBuffer(vbo, size);
}

void VertexDrawable::setPrimitiveMode(gl::GLenum primitiveMode)
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

void VertexDrawable::bindAttributes(const std::vector<gl::GLint> & attributeIndices)
{
    m_attributeIndices = attributeIndices;

    for (unsigned i = 0; i<m_attributeIndices.size(); ++i)
    {
        gl::GLint index = m_attributeIndices[i];
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

VertexDrawable::AttributeFormat Format(gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset, gl::GLboolean normalized)
{
    return VertexDrawable::AttributeFormat(size, type, normalized, relativeOffset, VertexDrawable::AttributeFormat::Float);
}

VertexDrawable::AttributeFormat FormatI(gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset)
{
    return VertexDrawable::AttributeFormat(size, type, gl::FALSE_, relativeOffset, VertexDrawable::AttributeFormat::Integer);
}

VertexDrawable::AttributeFormat FormatL(gl::GLint size, gl::GLenum type, gl::GLuint relativeOffset)
{
    return VertexDrawable::AttributeFormat(size, type, gl::FALSE_, relativeOffset, VertexDrawable::AttributeFormat::Long);
}

} // namespace glowutils
