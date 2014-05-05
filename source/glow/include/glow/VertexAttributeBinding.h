#pragma once

#include <GL/glew.h>

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
    ,   const GLint bindingIndex);

    const VertexArrayObject * vao() const;
    VertexArrayObject * vao();

	void setAttribute(GLint attributeIndex);
	void setBuffer(
        const Buffer * vbo
    ,   GLint baseoffset
    ,   GLint stride);

	void setFormat(
        GLint size
    ,   GLenum type
    ,   GLboolean normalized = GL_FALSE
    ,   GLuint relativeoffset = 0);
	void setIFormat(
        GLint size
    ,   GLenum type
    ,   GLuint relativeoffset = 0);
	void setLFormat(
        GLint size
    ,   GLenum type
    ,   GLuint relativeoffset = 0);

    GLint attributeIndex() const;
    GLint bindingIndex() const;
    const Buffer * buffer() const;

protected:
    ~VertexAttributeBinding();

    VertexArrayObject * m_vao; // TODO: weak_ptr?
   
    GLint m_bindingIndex;
    GLint m_attributeIndex;
    
    const Buffer * m_vbo;

    VertexAttributeBindingImplementation * m_implementation;
};

} // namespace glow
