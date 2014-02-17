#pragma once

#include <map>
#include <vector>

#include <glow/glow.h>
#include <glow/Object.h>
#include <glow/ref_ptr.h>

namespace glow 
{
class ObjectVisitor;
class Buffer;
class VertexAttributeBinding;

// http://www.opengl.org/wiki/Vertex_Array_Object
class GLOW_API VertexArrayObject : public Object
{
public:
	VertexArrayObject();
	VertexArrayObject(GLuint id, bool ownsGLObject = true);
	virtual ~VertexArrayObject();

    virtual void accept(ObjectVisitor & visitor) override;

	void bind();
	void unbind();

	VertexAttributeBinding * binding(GLuint bindingIndex);

	void enable(GLint attributeIndex);
	void disable(GLint attributeIndex);

	std::vector<VertexAttributeBinding *> bindings();

    // drawing
    void drawArrays(
        GLenum mode
    ,   GLint first
    ,   GLsizei count);

    void drawArraysInstanced(
        GLenum mode
    ,   GLint first
    ,   GLsizei count
    ,   GLsizei instanceCount);

    void drawArraysInstancedBaseInstance(
        GLenum mode
    ,   GLint first
    ,   GLsizei count
    ,   GLsizei instanceCount
    ,   GLuint baseInstance);

    void drawArraysIndirect(
        GLenum mode
    ,   const void * indirect);

    void multiDrawArrays(
        GLenum mode
    ,   GLint * first
    ,   const GLsizei * count
    ,   GLsizei drawCount);
    
    void multiDrawArraysIndirect(
        GLenum mode
    ,   const void * indirect
    ,   GLsizei drawCount
    ,   GLsizei stride);

    void drawElements(
        GLenum mode
    ,   GLsizei count
    ,   GLenum type
    ,   const void * indices);

    void drawElementsBaseVertex(
        GLenum mode
    ,   GLsizei count
    ,   GLenum type
    ,   const void * indices
    ,   GLint baseVertex);
    
    void drawElementsInstanced(
        GLenum mode
    ,   GLsizei count
    ,   GLenum type
    ,   const void * indices
    ,   GLsizei primitiveCount);
    
    void drawElementsInstancedBaseInstance(
        GLenum mode
    ,   GLsizei count
    ,   GLenum type
    ,   const void * indices
    ,   GLsizei instanceCount
    ,   GLuint baseInstance);
   
    void drawElementsInstancedBaseVertex(
        GLenum mode
    ,   GLsizei count
    ,   GLenum type
    ,   const void * indices
    ,   GLsizei instanceCount
    ,   GLint baseVertex);

    void drawElementsInstancedBaseVertexBaseInstance(
        GLenum mode
    ,   GLsizei count
    ,   GLenum type
    ,   const void * indices
    ,   GLsizei instanceCount
    ,   GLint baseVertex
    ,   GLuint baseInstance);

    void multiDrawElements(
        GLenum mode
    ,   const GLsizei * count
    ,   GLenum type
    ,   const void ** indices
    ,   GLsizei drawCount);
    
    void multiDrawElementsBaseVertex(
        GLenum mode
    ,   const GLsizei * count
    ,   GLenum type
    ,   const void ** indices
    ,   GLsizei primitiveCount
    ,   GLint * baseVertex);

    void multiDrawElementsIndirect(
        GLenum mode
    ,   GLenum type
    ,   const void * indirect
    ,   GLsizei drawCount
    ,   GLsizei stride);

    void drawRangeElements(
        GLenum mode
    ,   GLuint start
    ,   GLuint end
    ,   GLsizei count
    ,   GLenum type
    ,   const void * indices);

    void drawRangeElementsBaseVertex(
        GLenum mode
    ,   GLuint start
    ,   GLuint end
    ,   GLsizei count
    ,   GLenum type
    ,   const void * indices
    ,   GLint baseVertex);

protected:
    static GLuint genVertexArray();

protected:
	GLuint m_nextBindingIndex;
	std::map<GLuint, ref_ptr<VertexAttributeBinding>> m_bindings;

};

} // namespace glow
