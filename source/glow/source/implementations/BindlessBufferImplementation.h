#pragma once

#include "AbstractBufferImplementation.h"

namespace glow
{

class Buffer;

class BindlessBufferImplementation : public AbstractBufferImplementation
{
public:
    virtual void * map(const Buffer * buffer, gl::GLenum access) const override;
    virtual void * mapRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length, gl::GLbitfield access) const override;
    virtual bool unmap(const Buffer * buffer) const override;

    virtual void setData(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage) const override;
    virtual void setSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid * data) const override;
    virtual void setStorage(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLbitfield flags) const override;

    virtual void copySubData(const Buffer * buffer, Buffer * other, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const override;

    virtual void getParameter(const Buffer * buffer, gl::GLenum pname, gl::GLint * data) const override;

    virtual void clearData(const Buffer * buffer, gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data) const override;
    virtual void clearSubData(const Buffer * buffer, gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data) const override;

    virtual void flushMappedRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length) const override;
};

} // namespace glow
