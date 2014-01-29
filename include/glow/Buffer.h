#pragma once

#include <glow/glow.h>

#include <glow/Object.h>

namespace glow
{
class AbstractArray;

/** \brief Wrapper for OpenGL buffer objects.
    
    A buffer has a target, which can later be changed. Each buffer can be bound
    and unbound (bind(), unbind()). To set the buffer to a specific size, use
    setData(). To access the data of a buffer directly, you can use map().
    Buffers can be used for OpenGL draw calls, which are encapsulated within
    drawArrays() and drawElements(), but that doesn't guarantee that OpenGL 
    will use this buffer for drawing. The current bound VertexArrayObject and 
    Program will specify the render pipeline and data.

    \code{.cpp}

    	Buffer* buffer = new Buffer(GL_SHADER_STORAGE_BUFFER);
    
    	buffer->setData(sizeof(glm::vec4) * 100, nullptr, GL_DYNAMIC_DRAW); // allocate 100 vec4

    \endcode
    
    \see http://www.opengl.org/wiki/Buffer_Object
 */
class GLOW_API Buffer : public Object
{
public:
	Buffer();
	Buffer(GLenum target);
	Buffer(GLuint id, GLenum target);
	virtual ~Buffer();

    virtual void accept(ObjectVisitor & visitor) override;

	void bind();
	void bind(GLenum target);
	void unbind();
    static void unbind(GLenum target);

	void setData(
        const AbstractArray & data
    ,   GLenum usage = GL_STATIC_DRAW);
	void setData(
        GLsizei size
    ,   const GLvoid * data = nullptr
    ,   GLenum usage = GL_STATIC_DRAW);

    /**
     * Wrapper for OpenGL function:
     *
     *      glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
     */
    void setSubData (const AbstractArray& data, GLintptr offset = 0);
    
    /**
     * Wrapper for OpenGL function:
     *
     *      glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
     */
    void setSubData (GLsizeiptr size, GLintptr offset = 0, const GLvoid* data = nullptr);

	GLint getParameter(GLenum pname);

    // mapping
	void * map(GLenum access = GL_READ_ONLY);
	void * map(
        GLenum target
    ,   GLenum access);
    void * mapRange(
        GLintptr offset
    ,   GLsizeiptr length
    ,   GLbitfield access);

	void unmap();

	// indexed buffer binding
	void bindBase(
        GLenum target
    ,   GLuint index);
	void bindRange(
        GLenum target
    ,   GLuint index
    ,   GLintptr offset
    ,   GLsizeiptr size);

	// copying
    static void copySubData(
        GLenum     readTarget
    ,   GLenum     writeTarget
    ,   GLintptr   readOffset
    ,   GLintptr   writeOffset
    ,   GLsizeiptr size);
	void copySubData(
        GLenum     writeTarget
    ,   GLintptr   readOffset
    ,   GLintptr   writeOffset
    ,   GLsizeiptr size);
	void copySubData(
        GLenum     writeTarget
    ,   GLsizeiptr size);
	void copySubData(
        glow::Buffer * buffer
    ,   GLintptr   readOffset
    ,   GLintptr   writeOffset
    ,   GLsizeiptr size);
	void copySubData(
        glow::Buffer * buffer
    ,   GLsizeiptr size);
	void copyData(
        glow::Buffer * buffer
    ,   GLsizeiptr size
    ,   GLenum usage = GL_STATIC_DRAW);

    // clear
    void clearData(
        GLenum internalformat
    ,   GLenum format
    ,   GLenum type
    ,   const void * data = nullptr);

    void clearSubData(
        GLenum internalformat
    ,   GLintptr offset
    ,   GLsizeiptr size
    ,   GLenum format
    ,   GLenum type
    ,   const void * data = nullptr);

protected:
    GLenum m_target;

	static GLuint genBuffer();
};

} // namespace glow
