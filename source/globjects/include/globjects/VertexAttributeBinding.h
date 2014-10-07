#pragma once

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

    gl::GLint attributeIndex() const;
    gl::GLint bindingIndex() const;
    const Buffer * buffer() const;

protected:
    ~VertexAttributeBinding();

protected:
    VertexArray * m_vao; // TODO: weak_ptr?
   
    gl::GLint m_bindingIndex;
    gl::GLint m_attributeIndex;
    
    const Buffer * m_vbo;

    mutable void * m_bindingData;
};

} // namespace globjects
