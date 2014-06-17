#pragma once

#include <vector>
#include <array>

#include <glbinding/types.h>
#include <glbinding/constants.h>

#include <glow/glow_api.h>
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
 * Buffer * buffer = new Buffer(gl::GL_SHADER_STORAGE_BUFFER);
 * buffer->setData(sizeof(glm::vec4) * 100, nullptr, gl::GL_DYNAMIC_DRAW); // allocate 100 vec4
 * \endcode
 *
 * \see http://www.opengl.org/wiki/Buffer_Object
 */
class GLOW_API Buffer : public Object
{
public:
    /**
     * Sets the target that is used for binding buffers to call state changing OpenGL functions.
     * This has an effect only when GL_EXT_direct_state_access is not available.
     * Usually this target never has to be changed unless you want to ensure that a certain binding target will not be used.
     * @param target
     */
    static void setWorkingTarget(gl::GLenum target);

public:
    /**
     * Creates a new OpenGL buffer object.
     */
    Buffer();
    /**
     * Creates a buffer with an external id. This object does not own the associated OpenGL object and
     * will not delete it in the destructor.
     * @param id an external OpenGL buffer id
     */
    static Buffer * fromId(gl::GLuint id, bool takeOwnership = false);

    /**
     * Implements the visitor pattern.
     * @param visitor The visitor on which visitBuffer will be called.
     */
    virtual void accept(ObjectVisitor & visitor) override;

    /**
     * Binds the buffer to target.
     * @param target the target for binding
     * \see https://www.opengl.org/sdk/docs/man4/xhtml/gl::glBindBuffer.xml
     */
    void bind(gl::GLenum target) const;

    /**
     * Unbinds a specific target, i.e. binds a 0 id to the target.
     * @param target the target for unbinding
     */
    static void unbind(gl::GLenum target);
    /**
     * Unbinds the buffer bound to the target and index.
     * @param target the target for unbinding
     * @param index the index for unbinding
     */
    static void unbind(gl::GLenum target, gl::GLuint index);

    /**
     * Wraps the OpenGL function glBufferData.
     * Creates video memory for the buffer.
     * @param size size of the new memory in bytes
     * @param data memory location containing the data. If data is nullptr, uninitalized memory will be created.
     * @param usage used as a performance hint on how the buffer is used
     * \see https://www.opengl.org/sdk/docs/man4/xhtml/glBufferData.xml
     */
    void setData(gl::GLsizeiptr size, const gl::GLvoid * data = nullptr, gl::GLenum usage = gl::GL_STATIC_DRAW);
    /**
     * Convenience method to simplify passing of data in form of an std::vector.
     */
    template <typename T>
    void setData(const std::vector<T> & data, gl::GLenum usage = gl::GL_STATIC_DRAW);
    /**
     * Convenience method to simplify passing of data in form of an std::array.
     */
    template <typename T, std::size_t Count>
    void setData(const std::array<T, Count> & data, gl::GLenum usage = gl::GL_STATIC_DRAW);
    /**
     * Wraps the OpenGL function glBufferSubData.
     * Writes data only to a defined area of the memory.
     * @param size size of memory in bytes
     * @param offset offset from the beginning of the buffer in bytes
     * @param data memory location containing the data
     * \see http://www.opengl.org/sdk/docs/man/xhtml/glBufferSubData.xml
     */
    void setSubData(gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid* data = nullptr);
    /**
     * Convenience method to simplify passing of data in form of an std::vector.
     */
    template <typename T>
    void setSubData(const std::vector<T> & data, gl::GLintptr offset = 0);
    /**
     * Convenience method to simplify passing of data in form of an std::array.
     */
    template <typename T, std::size_t Count>
    void setSubData(const std::array<T, Count> & data, gl::GLintptr offset = 0);

    /**
     * Wraps the OpenGL function glBufferStorage.
     * @param size size of the new memory in bytes
     * @param data data memory location containing the data.
     * @param flags flags indicating usage
     * \see www.opengl.org/sdk/docs/man/xhtml/glBufferStorage.xml
     */
    void setStorage(gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLbitfield flags);
    /**
     * Convenience method to simplify passing of data in form of an std::vector.
     */
    template <typename T>
    void setStorage(const std::vector<T> & data, gl::GLbitfield flags);
    /**
     * Convenience method to simplify passing of data in form of an std::array.
     */
    template <typename T, std::size_t Count>
    void setStorage(const std::array<T, Count> & data, gl::GLbitfield flags);

    /**
     * Wraps the OpenGL function gl::glGetBufferParameter.
     * Queries OpenGL for internal state of the buffer.
     * @param pname name of the parameter, e.g. gl::glBUFFER_SIZE
     * @return integer value for the parameter
     * \see http://www.opengl.org/sdk/docs/man/xhtml/gl::glGetBufferParameter.xml
     */
    gl::GLint getParameter(gl::GLenum pname) const;

    /**
     * Maps the Buffer's memory read only.
     * @return a pointer to the mapped memory
     */
    const void * map() const;

    /**
     * Maps the Buffer's memory using the internal target.
     * @param access specifies reading/writing access
     * @return a pointer to the mapped memory
     */
    void * map(gl::GLenum access = gl::GL_READ_ONLY);
    /**
     * Wraps the OpenGL function glMapBufferRange.
     * Maps only a range of the buffers memory.
     * @param offset offset from the beginning of the buffer data in bytes.
     * @param length length of the range in bytes.
     * @param access bitfield of desired access flags
     * @return pointer to the mapped memory
     * \see http://www.opengl.org/sdk/docs/man/xhtml/glMapBufferRange.xml
     */
    void * mapRange(gl::GLintptr offset, gl::GLsizeiptr length, gl::GLbitfield access);
    /**
     * Wraps the OpenGL function glUnmapBuffer.
     * \see http://www.opengl.org/sdk/docs/man3/xhtml/glMapBuffer.xml
     */
    bool unmap() const;

    /**
     * @brief Wraps the OpenGL function  glFlushMappedBufferRange.
     * @param offset offset from the beginning of the buffer data in bytes.
     * @param length length of the range in bytes
     * \see http://www.opengl.org/sdk/docs/man/html/glFlushMappedBufferRange.xhtml
     */
    void flushMappedRange(gl::GLintptr offset, gl::GLsizeiptr length);

    /**
     * Wraps the OpenGL function gl::glBindBufferBase.
     * \see http://www.opengl.org/sdk/docs/man/xhtml/gl::glBindBufferBase.xml
     */
    void bindBase(gl::GLenum target, gl::GLuint index) const;
    /**
     * Wraps the OpenGL function gl::glBindBufferRange.
     * \see http://www.opengl.org/sdk/docs/man3/xhtml/gl::glBindBufferRange.xml
     */
    void bindRange(gl::GLenum target, gl::GLuint index, gl::GLintptr offset, gl::GLsizeiptr size) const;

    /**
     * Wraps the OpenGL function glCopyBufferSubData.
     * @param readOffset offset in bytes in read buffer
     * @param writeOffset offset in bytes in write buffer
     * @param size size of the data to be copies in bytes
     * \see http://www.opengl.org/sdk/docs/man3/xhtml/glCopyBufferSubData.xml
     */
    void copySubData(Buffer * buffer, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const;
    /**
     * Convenience method.
     * Both readOffset and writeOffset are 0.
     */
    void copySubData(Buffer * buffer, gl::GLsizeiptr size) const;
    /**
     * Creates new uninitialized memory to fit size (using usage), then
     * copies the contents of buffer to this buffer's new memory.
     * @param buffer buffer from which content is copied
     * @param size size of the data to be copied
     * @param usage buffer usage
     */
    void copyData(Buffer * buffer, gl::GLsizeiptr size, gl::GLenum usage = gl::GL_STATIC_DRAW) const;

    /**
     * Wraps the OpenGL function gl::glClearBufferData.
     * Clears the Buffer's data by filling it with the value in data, which has to be long enough to match format.
     * @param data up to 4 components of the vector value to fill the buffer with
     * \see http://www.opengl.org/sdk/docs/man/xhtml/gl::glClearBufferData.xml
     */
    void clearData(gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data = nullptr);
    /**
     * Wraps the OpenGL function gl::glClearBufferSubData.
     * @param offset offset in bytes
     * @param size size in bytes
     * \see https://www.opengl.org/sdk/docs/man4/xhtml/gl::glClearBufferSubData.xml
     */
    void clearSubData(gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data = nullptr);

protected:
    /**
     * Creates a buffer with an external id.
     * @param id an external OpenGL buffer id
     */
    Buffer(gl::GLuint id, bool takeOwnership);
    /**
     * @brief ~Buffer
     * Automatically deletes the associated OpenGL buffer unless the object was created with an external id.
     * \see https://www.opengl.org/sdk/docs/man4/xhtml/gl::glDeleteBuffers.xml
     */
    virtual ~Buffer();

    /**
     * Wraps the OpenGL function gl::glGenBuffers.
     * @return id of a newly created OpenGL buffer
     * /see https://www.opengl.org/sdk/docs/man4/xhtml/gl::glGenBuffers.xml
     */
    static gl::GLuint genBuffer();
};

} // namespace glow

#include <glow/Buffer.hpp>
