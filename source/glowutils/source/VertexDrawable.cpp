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

VertexDrawable::VertexDrawable(glow::Buffer* vbo, GLint baseOffset, GLint stride, GLint size, GLenum primitiveMode)
: m_vao(new glow::VertexArrayObject)
, m_vbo(vbo)
, m_baseOffset(baseOffset)
, m_stride(stride)
, m_size(size)
, m_primitiveMode(primitiveMode)
{
}

void VertexDrawable::setFormats(const std::vector<AttributeFormat> & formats)
{
    for (unsigned i = 0; i<formats.size(); ++i)
    {
        auto binding = m_vao->binding(i);
        binding->setAttribute(i);
        binding->setBuffer(m_vbo, m_baseOffset, m_stride);
        formats[i].setTo(binding);

        m_vao->enable(i);
    }
}

void VertexDrawable::bindAttributes(const std::vector<GLint> & attributeIndices)
{
    for (unsigned i = 0; i<attributeIndices.size(); ++i)
    {
        m_vao->binding(i)->setAttribute(attributeIndices[i]);
    }
}

void VertexDrawable::draw()
{
    m_vao->drawArrays(m_primitiveMode, 0, m_size);
}

} // namespace glowutils
