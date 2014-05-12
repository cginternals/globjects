#pragma once

#include <glbinding/types.h>
#include <glbinding/constants.h>

#include <glowbase/Referenced.h>
#include <glowbase/ref_ptr.h>

#include <glow/glow_api.h>

namespace glow 
{

class Buffer;
class VertexArrayObject;
class VertexAttributeBindingImplementation;

class GLOW_API VertexAttributeBinding : public Referenced
{
	friend class VertexAttributeBindingImplementation;

public:
	VertexAttributeBinding(
        VertexArrayObject * vao
    ,   const gl::GLint bindingIndex);

    const VertexArrayObject * vao() const;
    VertexArrayObject * vao();

	void setAttribute(gl::GLint attributeIndex);
	void setBuffer(
        const Buffer * vbo
    ,   gl::GLint baseoffset
    ,   gl::GLint stride);

	void setFormat(
        gl::GLint size
    ,   gl::GLenum type
    ,   gl::GLboolean normalized = static_cast<gl::GLboolean>(gl::FALSE_)
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

    VertexArrayObject * m_vao; // TODO: weak_ptr?
   
    gl::GLint m_bindingIndex;
    gl::GLint m_attributeIndex;
    
    const Buffer * m_vbo;

    VertexAttributeBindingImplementation * m_implementation;
};

} // namespace glow
