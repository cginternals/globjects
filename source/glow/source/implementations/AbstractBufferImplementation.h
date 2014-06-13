#pragma once

#include <glbinding/types.h>

namespace glow
{

class Buffer;

class AbstractBufferImplementation
{
public:
    AbstractBufferImplementation();
    virtual ~AbstractBufferImplementation();

    static AbstractBufferImplementation * create();

    virtual void * map(const Buffer * buffer, gl::GLenum access) const = 0;
    virtual void * mapRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length, gl::GLbitfield access) const = 0;
    virtual bool unmap(const Buffer * buffer) const = 0;

    virtual void setData(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage) const = 0;
    virtual void setSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid * data) const = 0;
    virtual void setStorage(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLbitfield flags) const = 0;

    virtual void copySubData(const Buffer * buffer, Buffer * other, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const = 0;

    virtual void getParameter(const Buffer * buffer, gl::GLenum pname, gl::GLint * data) const = 0;

    virtual void clearData(const Buffer * buffer, gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data) const = 0;
    virtual void clearSubData(const Buffer * buffer, gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data) const = 0;

    virtual void flushMappedRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length) const = 0;
};

} // namespace glow
