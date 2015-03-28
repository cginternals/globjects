#pragma once

#include <vector>
#include <array>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>
#include <globjects/Object.h>

namespace globjects
{

/** \brief Wrapper for OpenGL buffer objects.
    
    The Buffer class encapsulates OpenGL buffer objects.
    Each buffer can be bound and unbound (bind(), unbind()).
    To fill the buffer use setData().
    To access the data of a buffer directly, you can use map().
    Buffers can be used for OpenGL draw calls, which are encapsulated within
    drawArrays() and drawElements() on VertexArrayObject,
    but that doesn't guarantee that OpenGL will use this buffer for drawing.
    The current bound VertexArrayObject and Program will specify the render pipeline and data.
    
    \code{.cpp}
    Buffer * buffer = new Buffer(gl::GL_SHADER_STORAGE_BUFFER);
    buffer->setData(sizeof(glm::vec4) * 100, nullptr, gl::GL_DYNAMIC_DRAW); // allocate 100 vec4
    \endcode
    
    \see http://www.opengl.org/wiki/Buffer_Object
*/
class GLOBJECTS_API Buffer : public Object
{
public:
    enum class BindlessImplementation
    {
        DirectStateAccessARB
    ,   DirectStateAccessEXT
    ,   Legacy
    };

    static void hintBindlessImplementation(BindlessImplementation impl);

    /** \brief Sets the target that is used for binding buffers to call state changing OpenGL functions.
        This has an effect only when GL_EXT_direct_state_access is not available.
        Usually this target never has to be changed unless you want to ensure that a certain binding target will not be used.
        \param target
    */
    static void setWorkingTarget(gl::GLenum target);

public:
    /** \brief Creates a new OpenGL buffer object.
    */
    Buffer();

    /** \brief Creates a buffer with an external id.
        This object does not own the associated OpenGL object and
        will not delete it in the destructor.
        \param id an external OpenGL buffer id
    */
    static Buffer * fromId(gl::GLuint id);

    /** \brief Implements the visitor pattern.
        \param visitor The visitor on which visitBuffer will be called.
    */
    virtual void accept(ObjectVisitor & visitor) override;

    /** \brief Binds the buffer to target.
        \param target the target for binding
        \see https://www.opengl.org/sdk/docs/man4/xhtml/gl::glBindBuffer.xml
    */
    void bind(gl::GLenum target) const;

    /** \brief Unbinds a specific target, i.e. binds a 0 id to the target.
        \param target the target for unbinding
    */
    static void unbind(gl::GLenum target);

    /** \brief Unbinds the buffer bound to the target and index.
        \param target the target for unbinding
        \param index the index for unbinding
    */
    static void unbind(gl::GLenum target, gl::GLuint index);

    /** \brief Wraps the OpenGL function glBufferData.
        Creates video memory for the buffer.
        \param size size of the new memory in bytes
        \param data memory location containing the data. If data is nullptr, uninitalized memory will be created.
        \param usage used as a performance hint on how the buffer is used
        \see https://www.opengl.org/sdk/docs/man4/xhtml/glBufferData.xml
    */
    void setData(gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage);
    
    /** \brief Convenience method to simplify passing of data in form of an std::vector.
    */
    template <typename T>
    void setData(const std::vector<T> & data, gl::GLenum usage);
    
    /** \brief Convenience method to simplify passing of data in form of an std::array.
    */
    template <typename T, std::size_t Count>
    void setData(const std::array<T, Count> & data, gl::GLenum usage);
    /** \brief Wraps the OpenGL function glBufferSubData.
        Writes data only to a defined area of the memory.
        \param size size of memory in bytes
        \param offset offset from the beginning of the buffer in bytes
        \param data memory location containing the data
        \see http://www.opengl.org/sdk/docs/man/xhtml/glBufferSubData.xml
    */
    void setSubData(gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid* data = nullptr);
    
    /** \brief Convenience method to simplify passing of data in form of an std::vector.
    */
    template <typename T>
    void setSubData(const std::vector<T> & data, gl::GLintptr offset = 0);
    
    /** \brief Convenience method to simplify passing of data in form of an std::array.
    */
    template <typename T, std::size_t Count>
    void setSubData(const std::array<T, Count> & data, gl::GLintptr offset = 0);

    /** \brief Wraps the OpenGL function glBufferStorage.
        \param size size of the new memory in bytes
        \param data data memory location containing the data.
        \param flags flags indicating usage
        \see www.opengl.org/sdk/docs/man/xhtml/glBufferStorage.xml
    */
    void setStorage(gl::GLsizeiptr size, const gl::GLvoid * data, gl::BufferStorageMask flags);
    
    /** \brief Convenience method to simplify passing of data in form of an std::vector.
    */
    template <typename T>
    void setStorage(const std::vector<T> & data, gl::BufferStorageMask flags);
    
    /** \brief Convenience method to simplify passing of data in form of an std::array.
    */
    template <typename T, std::size_t Count>
    void setStorage(const std::array<T, Count> & data, gl::BufferStorageMask flags);

    /** \brief Wraps the OpenGL function gl::glGetBufferParameter.
        Queries OpenGL for internal state of the buffer.
        \param pname name of the parameter, e.g. gl::GL_BUFFER_SIZE
        \return integer value for the parameter
        \see http://www.opengl.org/sdk/docs/man/xhtml/gl::glGetBufferParameter.xml
    */
    gl::GLint getParameter(gl::GLenum pname) const;

    /** \brief Wraps the OpenGL function gl::glGetBufferParameter for 64 bit data types.
        Queries OpenGL for internal state of the buffer.
        \param pname name of the parameter, e.g. gl::GL_BUFFER_SIZE
        \return integer value for the parameter
        \see http://www.opengl.org/sdk/docs/man/xhtml/gl::glGetBufferParameter.xml
    */
    gl::GLint64 getParameter64(gl::GLenum pname) const;

    /** \brief Maps the Buffer's memory read only.
        \return a pointer to the mapped memory
    */
    const void * map() const;

    /** \brief Maps the Buffer's memory using the internal target.
        \param access specifies reading/writing access
        \return a pointer to the mapped memory
    */
    void * map(gl::GLenum access);

    /** \brief Wraps the OpenGL function glMapBufferRange.
        Maps only a range of the buffers memory.
        \param offset offset from the beginning of the buffer data in bytes.
        \param length length of the range in bytes.
        \param access bitfield of desired access flags
        \return pointer to the mapped memory
        \see http://www.opengl.org/sdk/docs/man/xhtml/glMapBufferRange.xml
    */
    void * mapRange(gl::GLintptr offset, gl::GLsizeiptr length, gl::BufferAccessMask access);
    
    /** \brief Wraps the OpenGL function glUnmapBuffer.
        \see http://www.opengl.org/sdk/docs/man3/xhtml/glMapBuffer.xml
    */
    bool unmap() const;

    /** \brief Wraps the OpenGL function  glFlushMappedBufferRange.
        \param offset offset from the beginning of the buffer data in bytes.
        \param length length of the range in bytes
        \see http://www.opengl.org/sdk/docs/man/html/glFlushMappedBufferRange.xhtml
    */
    void flushMappedRange(gl::GLintptr offset, gl::GLsizeiptr length);

    /** \brief Wraps the OpenGL function gl::glBindBufferBase.
        \see http://www.opengl.org/sdk/docs/man/xhtml/gl::glBindBufferBase.xml
    */
    void bindBase(gl::GLenum target, gl::GLuint index) const;

    /** \brief Wraps the OpenGL function gl::glBindBufferRange.
        \see http://www.opengl.org/sdk/docs/man3/xhtml/gl::glBindBufferRange.xml
    */
    void bindRange(gl::GLenum target, gl::GLuint index, gl::GLintptr offset, gl::GLsizeiptr size) const;

    /** \brief Wraps the OpenGL function glCopyBufferSubData.
        \param readOffset offset in bytes in read buffer
        \param writeOffset offset in bytes in write buffer
        \param size size of the data to be copies in bytes
        \see http://www.opengl.org/sdk/docs/man3/xhtml/glCopyBufferSubData.xml
    */
    void copySubData(Buffer * buffer, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const;
    
    /** \brief Convenience method. Both readOffset and writeOffset are 0.
    */
    void copySubData(Buffer * buffer, gl::GLsizeiptr size) const;
    
    /** \brief Creates new uninitialized memory to fit size (using usage), then
        copies the contents of buffer to this buffer's new memory.
        \param buffer buffer from which content is copied
        \param size size of the data to be copied
        \param usage buffer usage
    */
    void copyData(Buffer * buffer, gl::GLsizeiptr size, gl::GLenum usage) const;

    /** \brief Wraps the OpenGL function gl::glClearBufferData.
        Clears the Buffer's data by filling it with the value in data, which has to be long enough to match format.
        \param data up to 4 components of the vector value to fill the buffer with
        \see http://www.opengl.org/sdk/docs/man/xhtml/gl::glClearBufferData.xml
    */
    void clearData(gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data = nullptr);
    
    /** \brief Wraps the OpenGL function gl::glClearBufferSubData.
        \param offset offset in bytes
        \param size size in bytes
        \see https://www.opengl.org/sdk/docs/man4/xhtml/gl::glClearBufferSubData.xml
    */
    void clearSubData(gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data = nullptr);

    const void * getPointer() const;
    void * getPointer();
    const void * getPointer(gl::GLenum pname) const;
    void * getPointer(gl::GLenum pname);

    void getSubData(gl::GLintptr offset, gl::GLsizeiptr size, void * data) const;

    template <typename T>
    const std::vector<T> getSubData(gl::GLsizeiptr size, gl::GLintptr offset = 0) const;
    
    /** \brief Convenience method to simplify passing of data in form of an std::array.
    */
    template <typename T, std::size_t Count>
    const std::array<T, Count> getSubData(gl::GLintptr offset = 0) const;

    /** \brief Wraps the OpenGL function gl::glInvalidateBufferData.
        \see https://www.opengl.org/sdk/docs/man/html/glInvalidateBufferData.xhtml
    */
    void invalidateData() const;

    /** \brief Wraps the OpenGL function gl::glInvalidateBufferSubData.
        \param offset offset in bytes
        \param size size in bytes
        \see https://www.opengl.org/sdk/docs/man/html/glInvalidateBufferSubData.xhtml
    */
    void invalidateSubData(gl::GLintptr offset, gl::GLsizeiptr length) const;

    virtual gl::GLenum objectType() const override;

protected:
    /** \brief Creates a buffer with an external id.
        \param id an external OpenGL buffer id
    */
    Buffer(IDResource * resource);

    /** Automatically deletes the associated OpenGL buffer unless the object was created with an external id.
        \see https://www.opengl.org/sdk/docs/man4/xhtml/gl::glDeleteBuffers.xml
    */
    virtual ~Buffer();
};

} // namespace globjects

#include <globjects/Buffer.hpp>
