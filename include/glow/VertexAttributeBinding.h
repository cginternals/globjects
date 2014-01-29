#pragma once

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>

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
	~VertexAttributeBinding();

	void setAttribute(GLint attributeIndex);
	void setBuffer(
        Buffer * vbo
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
    ,   GLuint relativeoffset = 0);

    GLint attributeIndex() const;
    GLint bindingIndex() const;
	Buffer * buffer() const;

protected:
    VertexArrayObject * m_vao; // TODO: weak_ptr?
   
    GLint m_bindingIndex;
    GLint m_attributeIndex;
    
    Buffer * m_vbo; // TODO: weak_ptr?

    VertexAttributeBindingImplementation * m_implementation;
};

} // namespace glow
