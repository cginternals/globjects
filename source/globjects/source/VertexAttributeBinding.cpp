#include <globjects/VertexAttributeBinding.h>

#include <cassert>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <globjects/VertexArray.h>
#include <globjects/globjects.h>

#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractVertexAttributeBindingImplementation.h"


using namespace gl;

namespace 
{
    globjects::AbstractVertexAttributeBindingImplementation & attributeImplementation()
    {
        return globjects::ImplementationRegistry::current().attributeImplementation();
    }
}

namespace globjects
{

VertexAttributeBinding::VertexAttributeBinding(
    VertexArray * vao
,   const GLint bindingIndex)
: m_vao(vao)
, m_bindingIndex(bindingIndex)
, m_attributeIndex(0)
, m_vbo(nullptr)
, m_bindingData(nullptr)
{
    assert(vao != nullptr);

	// setAttribute(bindingIndex); // as default - this leads to errors when binding unknown type 
}

VertexAttributeBinding::~VertexAttributeBinding()
{
}

const VertexArray * VertexAttributeBinding::vao() const
{
    return m_vao;
}

VertexArray * VertexAttributeBinding::vao()
{
    return m_vao;
}

void VertexAttributeBinding::setDivisor(GLint divisor)
{
    attributeImplementation().setAttributeDivisor(this, divisor);
}

void VertexAttributeBinding::setAttribute(GLint attributeIndex)
{
    m_attributeIndex = attributeIndex;
    attributeImplementation().bindAttribute(this, attributeIndex);
}

GLint VertexAttributeBinding::attributeIndex() const
{
    return m_attributeIndex;
}

GLint VertexAttributeBinding::bindingIndex() const
{
    return m_bindingIndex;
}

const Buffer * VertexAttributeBinding::buffer() const
{
    return m_vbo;
}

void VertexAttributeBinding::setBuffer(const Buffer * vbo, const GLint baseoffset, const GLint stride)
{
    m_vbo = vbo;
    attributeImplementation().bindBuffer(this, vbo, baseoffset, stride);
}

void VertexAttributeBinding::setFormat(const GLint size, const GLenum type, const GLboolean normalized, const GLuint relativeoffset)
{
    attributeImplementation().setFormat(this, size, type, normalized, relativeoffset);
}

void VertexAttributeBinding::setIFormat(const GLint size, const GLenum type, const GLuint relativeoffset)
{
    attributeImplementation().setIFormat(this, size, type, relativeoffset);
}

void VertexAttributeBinding::setLFormat(const GLint size, const GLenum type, const GLuint relativeoffset)
{
    attributeImplementation().setLFormat(this, size, type, relativeoffset);
}

void VertexAttributeBinding::setValue(gl::GLboolean value)
{
    glVertexAttribI1i(bindingIndex(), static_cast<GLint>(value));
}

void VertexAttributeBinding::setValue(gl::GLbyte value)
{
    glVertexAttribI1i(bindingIndex(), static_cast<GLint>(value));
}

void VertexAttributeBinding::setValue(gl::GLshort value)
{
    glVertexAttribI1i(bindingIndex(), static_cast<GLint>(value));
}

void VertexAttributeBinding::setValue(gl::GLint value)
{
    glVertexAttribI1i(bindingIndex(), value);
}

void VertexAttributeBinding::setValue(gl::GLint64 value)
{
    glVertexAttribL1i64NV(bindingIndex(), value);
}

void VertexAttributeBinding::setValue(gl::GLfloat value)
{
    glVertexAttrib1f(bindingIndex(), value);
}

void VertexAttributeBinding::setValue(gl::GLdouble value)
{
    glVertexAttrib1d(bindingIndex(), value);
    glVertexAttribL1d(bindingIndex(), value);
}

void VertexAttributeBinding::setValue(const glm::bvec2 value)
{
    glVertexAttribI2i(bindingIndex(), static_cast<GLint>(value.x), static_cast<GLint>(value.y));
}

void VertexAttributeBinding::setValue(const glm::ivec2 value)
{
    glVertexAttribI2i(bindingIndex(), value.x, value.y);
}

void VertexAttributeBinding::setValue(const glm::vec2 value)
{
    glVertexAttrib2f(bindingIndex(), value.x, value.y);
}

void VertexAttributeBinding::setValue(const glm::dvec2 value)
{
    glVertexAttribL2d(bindingIndex(), value.x, value.y);
}

void VertexAttributeBinding::setValue(const glm::bvec3 value)
{
    glVertexAttribI3i(bindingIndex(), static_cast<GLint>(value.x), static_cast<GLint>(value.y), static_cast<GLint>(value.z));
}

void VertexAttributeBinding::setValue(const glm::ivec3 value)
{
    glVertexAttribI3i(bindingIndex(), value.x, value.y, value.z);
}

void VertexAttributeBinding::setValue(const glm::vec3 value)
{
    glVertexAttrib3f(bindingIndex(), value.x, value.y, value.z);
}

void VertexAttributeBinding::setValue(const glm::dvec3 value)
{
    glVertexAttribL3d(bindingIndex(), value.x, value.y, value.z);
}

void VertexAttributeBinding::setValue(const glm::bvec4 value)
{
    glVertexAttribI4i(bindingIndex(), static_cast<GLint>(value.x), static_cast<GLint>(value.y), static_cast<GLint>(value.z), static_cast<GLint>(value.w));
}

void VertexAttributeBinding::setValue(const glm::ivec4 value)
{
    glVertexAttribI4i(bindingIndex(), value.x, value.y, value.z, value.w);
}

void VertexAttributeBinding::setValue(const glm::vec4 value)
{
    glVertexAttrib4f(bindingIndex(), value.x, value.y, value.z, value.w);
}

void VertexAttributeBinding::setValue(const glm::dvec4 value)
{
    glVertexAttribL4d(bindingIndex(), value.x, value.y, value.z, value.w);
}


} // namespace globjects
