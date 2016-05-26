
#include <globjects/VertexAttributeBinding.h>

#include <cassert>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

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


} // namespace


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
    glVertexAttribL1d(bindingIndex(), value);
}

void VertexAttributeBinding::setValue(const glm::bvec2 & value)
{
    glVertexAttribI2i(bindingIndex(), static_cast<GLint>(value.x), static_cast<GLint>(value.y));
}

void VertexAttributeBinding::setValue(const glm::ivec2 & value)
{
    glVertexAttribI2i(bindingIndex(), value.x, value.y);
}

void VertexAttributeBinding::setValue(const glm::vec2 & value)
{
    glVertexAttrib2f(bindingIndex(), value.x, value.y);
}

void VertexAttributeBinding::setValue(const glm::dvec2 & value)
{
    glVertexAttribL2d(bindingIndex(), value.x, value.y);
}

void VertexAttributeBinding::setValue(const glm::bvec3 & value)
{
    glVertexAttribI3i(bindingIndex(), static_cast<GLint>(value.x), static_cast<GLint>(value.y), static_cast<GLint>(value.z));
}

void VertexAttributeBinding::setValue(const glm::ivec3 & value)
{
    glVertexAttribI3i(bindingIndex(), value.x, value.y, value.z);
}

void VertexAttributeBinding::setValue(const glm::vec3 & value)
{
    glVertexAttrib3f(bindingIndex(), value.x, value.y, value.z);
}

void VertexAttributeBinding::setValue(const glm::dvec3 & value)
{
    glVertexAttribL3d(bindingIndex(), value.x, value.y, value.z);
}

void VertexAttributeBinding::setValue(const glm::bvec4 & value)
{
    glVertexAttribI4i(bindingIndex(), static_cast<GLint>(value.x), static_cast<GLint>(value.y), static_cast<GLint>(value.z), static_cast<GLint>(value.w));
}

void VertexAttributeBinding::setValue(const glm::ivec4 & value)
{
    glVertexAttribI4i(bindingIndex(), value.x, value.y, value.z, value.w);
}

void VertexAttributeBinding::setValue(const glm::vec4 & value)
{
    glVertexAttrib4f(bindingIndex(), value.x, value.y, value.z, value.w);
}

void VertexAttributeBinding::setValue(const glm::dvec4 & value)
{
    glVertexAttribL4d(bindingIndex(), value.x, value.y, value.z, value.w);
}

void VertexAttributeBinding::setValue(const glm::mat2 & value)
{
    glVertexAttrib2f(bindingIndex(), value[0][0], value[1][0]);
    glVertexAttrib2f(bindingIndex()+1, value[0][1], value[1][1]);
}

void VertexAttributeBinding::setValue(const glm::mat3 & value)
{
    glVertexAttrib3f(bindingIndex(), value[0][0], value[1][0], value[2][0]);
    glVertexAttrib3f(bindingIndex()+1, value[0][1], value[1][1], value[2][1]);
    glVertexAttrib3f(bindingIndex()+2, value[0][2], value[1][2], value[2][2]);
}

void VertexAttributeBinding::setValue(const glm::mat4 & value)
{
    glVertexAttrib4f(bindingIndex(), value[0][0], value[1][0], value[2][0], value[3][0]);
    glVertexAttrib4f(bindingIndex()+1, value[0][1], value[1][1], value[2][1], value[3][1]);
    glVertexAttrib4f(bindingIndex()+2, value[0][2], value[1][2], value[2][2], value[3][2]);
    glVertexAttrib4f(bindingIndex()+3, value[0][3], value[1][3], value[2][3], value[3][3]);
}

void VertexAttributeBinding::setValue(const glm::dmat2 & value)
{
    glVertexAttribL2d(bindingIndex(), value[0][0], value[1][0]);
    glVertexAttribL2d(bindingIndex()+1, value[0][1], value[1][1]);
}

void VertexAttributeBinding::setValue(const glm::dmat3 & value)
{
    glVertexAttribL3d(bindingIndex(), value[0][0], value[1][0], value[2][0]);
    glVertexAttribL3d(bindingIndex()+1, value[0][1], value[1][1], value[2][1]);
    glVertexAttribL3d(bindingIndex()+2, value[0][2], value[1][2], value[2][2]);
}

void VertexAttributeBinding::setValue(const glm::dmat4 & value)
{
    glVertexAttribL4d(bindingIndex(), value[0][0], value[1][0], value[2][0], value[3][0]);
    glVertexAttribL4d(bindingIndex()+1, value[0][1], value[1][1], value[2][1], value[3][1]);
    glVertexAttribL4d(bindingIndex()+2, value[0][2], value[1][2], value[2][2], value[3][2]);
    glVertexAttribL4d(bindingIndex()+3, value[0][3], value[1][3], value[2][3], value[3][3]);
}


} // namespace globjects
