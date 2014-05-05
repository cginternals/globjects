#pragma once

#include <GL/glew.h>

#include <glow/Buffer.h>

namespace glow
{

class AbstractBufferImplementation
{
public:
    AbstractBufferImplementation();
    virtual ~AbstractBufferImplementation();

    static AbstractBufferImplementation * create();

    virtual void * map(const Buffer * buffer, GLenum access) const = 0;
    virtual void * mapRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length, GLbitfield access) const = 0;
    virtual bool unmap(const Buffer * buffer) const = 0;

    virtual void setData(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLenum usage) const = 0;
    virtual void setSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data) const = 0;
    virtual void setStorage(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLbitfield flags) const = 0;

    virtual void copySubData(const Buffer * buffer, Buffer * other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const = 0;

    virtual void getParameter(const Buffer * buffer, GLenum pname, GLint * data) const = 0;

    virtual void clearData(const Buffer * buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const = 0;
    virtual void clearSubData(const Buffer * buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const = 0;
};

} // namespace glow
