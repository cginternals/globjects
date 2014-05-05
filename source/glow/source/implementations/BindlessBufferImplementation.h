#pragma once

#include <GL/glew.h>

#include <glow/Buffer.h>

#include "AbstractBufferImplementation.h"

namespace glow
{

class BindlessBufferImplementation : public AbstractBufferImplementation
{
public:
    virtual void * map(const Buffer * buffer, GLenum access) const override;
    virtual void * mapRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length, GLbitfield access) const override;
    virtual bool unmap(const Buffer * buffer) const override;

    virtual void setData(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLenum usage) const override;
    virtual void setSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data) const override;
    virtual void setStorage(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLbitfield flags) const override;

    virtual void copySubData(const Buffer * buffer, Buffer * other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const override;

    virtual void getParameter(const Buffer * buffer, GLenum pname, GLint * data) const override;

    virtual void clearData(const Buffer * buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const override;
    virtual void clearSubData(const Buffer * buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const override;
};

} // namespace glow
