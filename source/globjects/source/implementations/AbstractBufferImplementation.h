#pragma once

#include <glbinding/gl/types.h>

#include <globjects/Buffer.h>


namespace globjects
{

class Buffer;

class AbstractBufferImplementation
{
public:
    AbstractBufferImplementation();
    virtual ~AbstractBufferImplementation();

    static AbstractBufferImplementation * get(Buffer::BindlessImplementation impl = 
        Buffer::BindlessImplementation::DirectStateAccessARB);

    virtual gl::GLuint create() const = 0;
    virtual void destroy(gl::GLuint id) const = 0;

    virtual void * map(const Buffer * buffer, gl::GLenum access) const = 0;
    virtual void * mapRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length, gl::BufferAccessMask access) const = 0;
    virtual bool unmap(const Buffer * buffer) const = 0;

    virtual void setData(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage) const = 0;
    virtual void setSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid * data) const = 0;
    virtual void setStorage(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::BufferStorageMask flags) const = 0;

    virtual void copySubData(const Buffer * buffer, Buffer * other, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const = 0;

    virtual gl::GLint getParameter(const Buffer * buffer, gl::GLenum pname) const = 0;
    virtual gl::GLint64 getParameter64(const Buffer * buffer, gl::GLenum pname) const = 0;
    virtual void * getPointer(const Buffer * buffer, gl::GLenum pname) const = 0;

    virtual void clearData(const Buffer * buffer, gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data) const = 0;
    virtual void clearSubData(const Buffer * buffer, gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data) const = 0;

    virtual void flushMappedRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length) const = 0;

    virtual void getBufferSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLvoid * data) const = 0;

    virtual void invalidateData(const Buffer * buffer) const = 0;
    virtual void invalidateSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length) const = 0;
};

} // namespace globjects
