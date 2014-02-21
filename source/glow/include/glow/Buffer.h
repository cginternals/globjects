#pragma once

#include <vector>
#include <array>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Object.h>

namespace glow
{

/** \brief Wrapper for OpenGL buffer objects.
 *
 * The Buffer class encapsulates OpenGL buffer objects.
 * Each buffer can be bound and unbound (bind(), unbind()).
 * To fill the buffer use setData().
 * To access the data of a buffer directly, you can use map().
 * Buffers can be used for OpenGL draw calls, which are encapsulated within
 * drawArrays() and drawElements() on VertexArrayObject,
 * but that doesn't guarantee that OpenGL will use this buffer for drawing.
 * The current bound VertexArrayObject and Program will specify the render pipeline and data.
 *
 * \code{.cpp}
 * Buffer* buffer = new Buffer(GL_SHADER_STORAGE_BUFFER);
 * buffer->setData(sizeof(glm::vec4) * 100, nullptr, GL_DYNAMIC_DRAW); // allocate 100 vec4
 * \endcode
 *
 * \see http://www.opengl.org/wiki/Buffer_Object
 */
class GLOW_API Buffer : public Object
{
public:
    /**
     * Creates a new OpenGL buffer object with an undefined target.
     * You should not call bind() without a target parameter afterwards as the behavior will be undefined.
     * Most other methods will break too if the buffer is not assigned a valid target before.
     * \see bind
     */
	Buffer();
    /**
     * Creates a buffer with a predefined target.
     * @param target will be used when bind() is called without parameter
     */
	Buffer(GLenum target);
    /**
     * Creates a buffer with an external id. This object does not own the associated OpenGL object and
     * will not delete it in the destructor.
     * @param id an external OpenGL buffer id
     * @param target will be used when bind() is called without parameter
     */
	Buffer(GLuint id, GLenum target);
    /**
     * @brief ~Buffer
     * Automatically deletes the associated OpenGL buffer unless the object was created with an external id.
     * \see https://www.opengl.org/sdk/docs/man4/xhtml/glDeleteBuffers.xml
     */
	virtual ~Buffer();

    /**
     * Implements the visitor pattern.
     * @param visitor The visitor on which visitBuffer will be called.
     */
    virtual void accept(ObjectVisitor & visitor) override;

    /**
     * Binds the buffer to the internal target.
     */
	void bind();
    /**
     * Binds the buffer to target and resets the internal target member which will be used for automatic binds.
     * @param target the target for binding
     * \see https://www.opengl.org/sdk/docs/man4/xhtml/glBindBuffer.xml
     */
	void bind(GLenum target);
    /**
     * Unbinds the buffer, i.e. the internal target will be bound to 0.
     * @param target the target for binding
     */
	void unbind();
    /**
     * Unbinds a specific target, i.e. binds a 0 id to the target.
     * @param target the target for unbinding
     */
    static void unbind(GLenum target);

    /**
     * Wraps the OpenGL function glBufferData.
     * Creates video memory for the buffer.
     * @param size size of the new memory in bytes
     * @param data memory location containing the data. If data is nullptr, uninitalized memory will be created.
     * @param usage used as a performance hint on how the buffer is used
     * \see https://www.opengl.org/sdk/docs/man4/xhtml/glBufferData.xml
     */
    void setData(GLsizeiptr size, const GLvoid * data = nullptr, GLenum usage = GL_STATIC_DRAW);
    /**
     * Convenience method to simplify passing of data in form of an std::vector.
     */
    template <typename T>
    void setData(const std::vector<T> & data, GLenum usage = GL_STATIC_DRAW);
    /**
     * Convenience method to simplify passing of data in form of an std::array.
     */
    template <typename T, std::size_t Count>
    void setData(const std::array<T, Count> & data, GLenum usage = GL_STATIC_DRAW);
    /**
     * Wraps the OpenGL function glBufferSubData.
     * Writes data only to a defined area of the memory.
     * @param size size of memory in bytes
     * @param offset offset from the beginning of the buffer in bytes
     * @param data memory location containing the data
     * \see http://www.opengl.org/sdk/docs/man/xhtml/glBufferSubData.xml
     */
    void setSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data = nullptr);
    /**
     * Convenience method to simplify passing of data in form of an std::vector.
     */
    template <typename T>
    void setSubData(const std::vector<T> & data, GLintptr offset = 0);
    /**
     * Convenience method to simplify passing of data in form of an std::array.
     */
    template <typename T, std::size_t Count>
    void setSubData(const std::array<T, Count> & data, GLintptr offset = 0);

    /**
     * Wraps the OpenGL function glBufferStorage.
     * @param size size of the new memory in bytes
     * @param data data memory location containing the data.
     * @param flags flags indicating usage
     * \see www.opengl.org/sdk/docs/man/xhtml/glBufferStorage.xml
     */
    void setStorage(GLsizeiptr size, const GLvoid * data, GLbitfield flags);
    /**
     * Convenience method to simplify passing of data in form of an std::vector.
     */
    template <typename T>
    void setStorage(const std::vector<T> & data, GLbitfield flags);
    /**
     * Convenience method to simplify passing of data in form of an std::array.
     */
    template <typename T, std::size_t Count>
    void setStorage(const std::array<T, Count> & data, GLbitfield flags);

    /**
     * Wraps the OpenGL function glGetBufferParameter.
     * Queries OpenGL for internal state of the buffer.
     * @param pname name of the parameter, e.g. GL_BUFFER_SIZE
     * @return integer value for the parameter
     * \see http://www.opengl.org/sdk/docs/man/xhtml/glGetBufferParameter.xml
     */
	GLint getParameter(GLenum pname);

    /**
     * Maps the Buffer's memory using the internal target.
     * @param access specifies reading/writing access
     * @return a pointer to the mapped memory
     */
    void * map(GLenum access = GL_READ_ONLY);
    /**
     * Wraps the OpenGL function glMapBufferRange.
     * Maps only a range of the buffers memory.
     * @param offset offset from the beginning of the buffer data in bytes.
     * @param length length of the range in bytes.
     * @param access bitfield of desired access flags
     * @return pointer to the mapped memory
     * \see http://www.opengl.org/sdk/docs/man/xhtml/glMapBufferRange.xml
     */
    void * mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access);
    /**
     * Wraps the OpenGL function glUnmapBuffer.
     * \see http://www.opengl.org/sdk/docs/man3/xhtml/glMapBuffer.xml
     */
    bool unmap();

    /**
     * Wraps the OpenGL function glBindBufferBase.
     * \see http://www.opengl.org/sdk/docs/man/xhtml/glBindBufferBase.xml
     */
    void bindBase(GLenum target, GLuint index);
    /**
     * Wraps the OpenGL function glBindBufferRange.
     * \see http://www.opengl.org/sdk/docs/man3/xhtml/glBindBufferRange.xml
     */
    void bindRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size);
    /**
     * Unbinds the buffer bound to the target and index.
     * @param target the target for unbinding
     * @param index the index for unbinding
     */
    static void unbindIndex(GLenum target, GLuint index);

    /**
     * Wraps the OpenGL function glCopyBufferSubData.
     * Copies contents of buffer bound to readTarget to the buffer bound to writeTarget.
     * @param readTarget target of buffer from which to read
     * @param writeTarget target of buffer in which to write copied data
     * @param readOffset offset in bytes in read buffer
     * @param writeOffset offset in bytes in write buffer
     * @param size size of the data to be copies in bytes
     * \see http://www.opengl.org/sdk/docs/man3/xhtml/glCopyBufferSubData.xml
     */
    static void copySubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
    /**
     * Convenience method.
     * Copies content of this buffer to whatever buffer is bound to writeTarget.
     * Uses GL_COPY_READ_BUFFER as readTarget.
     */
	void copySubData(GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
    /**
     * Convenience method.
     * Uses GL_COPY_READ_BUFFER as readTarget.
     * Both readOffset and writeOffset are 0.
     */
    void copySubData(GLenum writeTarget, GLsizeiptr size);
    /**
     * Convenience method.
     * Copies the contents of this Buffer to buffer.
     * Uses GL_COPY_WRITE_BUFFER as writeTarget, buffer will be bound to GL_COPY_WRITE_BUFFER.
     */
    void copySubData(glow::Buffer * buffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
    /**
     * Convenience method.
     * Both readOffset and writeOffset are 0.
     */
    void copySubData(glow::Buffer * buffer, GLsizeiptr size);
    /**
     * Creates new uninitialized memory to fit size (using usage), then
     * copies the contents of buffer to this buffer's new memory.
     * @param buffer buffer from which content is copied
     * @param size size of the data to be copied
     * @param usage buffer usage
     */
    void copyData(glow::Buffer * buffer, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

    /**
     * Wraps the OpenGL function glClearBufferData.
     * Clears the Buffer's data by filling it with the value in data, which has to be long enough to match format.
     * @param data up to 4 components of the vector value to fill the buffer with
     * \see http://www.opengl.org/sdk/docs/man/xhtml/glClearBufferData.xml
     */
    void clearData(GLenum internalformat, GLenum format, GLenum type, const void * data = nullptr);
    /**
     * Wraps the OpenGL function glClearBufferSubData.
     * @param offset offset in bytes
     * @param size size in bytes
     * \see https://www.opengl.org/sdk/docs/man4/xhtml/glClearBufferSubData.xml
     */
    void clearSubData(GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data = nullptr);
protected:
    /**
     * internal target used in bind().
     */
    GLenum m_target;

    /**
      * Cached boolean whether direct state access is available or not
      */
    bool m_directStateAccess; // TODO: move to per context cache

    /**
     * Wraps the OpenGL function glGenBuffers.
     * @return id of a newly created OpenGL buffer
     * /see https://www.opengl.org/sdk/docs/man4/xhtml/glGenBuffers.xml
     */
	static GLuint genBuffer();
};

} // namespace glow

#include <glow/Buffer.hpp>
