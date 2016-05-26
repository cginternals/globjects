
#pragma once

#include <glm/fwd.hpp>

#include <glbinding/gl/types.h>
#include <glbinding/gl/boolean.h>

#include <globjects/base/Referenced.h>

#include <globjects/globjects_api.h>


namespace globjects 
{


class Buffer;
class VertexArray;

class GLOBJECTS_API VertexAttributeBinding : public Referenced
{
    friend class AbstractVertexAttributeBindingImplementation;

public:
	VertexAttributeBinding(
        VertexArray * vao
    ,   const gl::GLint bindingIndex);

    const VertexArray * vao() const;
    VertexArray * vao();

    void setDivisor(gl::GLint divisor);

	void setAttribute(gl::GLint attributeIndex);

    gl::GLint attributeIndex() const;
    gl::GLint bindingIndex() const;

    /**
     * For VertexAttribPointer
     */
    const Buffer * buffer() const;
    void setBuffer(
        const Buffer * vbo
    ,   gl::GLint baseoffset
    ,   gl::GLint stride);

	void setFormat(
        gl::GLint size
    ,   gl::GLenum type
    ,   gl::GLboolean normalized = gl::GL_FALSE
    ,   gl::GLuint relativeoffset = 0);
	void setIFormat(
        gl::GLint size
    ,   gl::GLenum type
    ,   gl::GLuint relativeoffset = 0);
	void setLFormat(
        gl::GLint size
    ,   gl::GLenum type
    ,   gl::GLuint relativeoffset = 0);

    /**
     * For VertexAttrib
     */
    void setValue(gl::GLboolean value);
    void setValue(gl::GLbyte value);
    void setValue(gl::GLshort value);
    void setValue(gl::GLint value);
    void setValue(gl::GLint64 value);
    void setValue(gl::GLfloat value);
    void setValue(gl::GLdouble value);

    void setValue(const glm::bvec2 & value);
    void setValue(const glm::ivec2 & value);
    void setValue(const glm::vec2 & value);
    void setValue(const glm::dvec2 & value);

    void setValue(const glm::bvec3 & value);
    void setValue(const glm::ivec3 & value);
    void setValue(const glm::vec3 & value);
    void setValue(const glm::dvec3 & value);

    void setValue(const glm::bvec4 & value);
    void setValue(const glm::ivec4 & value);
    void setValue(const glm::vec4 & value);
    void setValue(const glm::dvec4 & value);

    void setValue(const glm::mat2 & value);
    void setValue(const glm::mat3 & value);
    void setValue(const glm::mat4 & value);
    void setValue(const glm::dmat2 & value);
    void setValue(const glm::dmat3 & value);
    void setValue(const glm::dmat4 & value);

protected:
    virtual ~VertexAttributeBinding();

protected:
    VertexArray * m_vao; // TODO: weak_ptr?
   
    gl::GLint m_bindingIndex;
    gl::GLint m_attributeIndex;
    
    const Buffer * m_vbo;

    mutable void * m_bindingData;
};


} // namespace globjects
