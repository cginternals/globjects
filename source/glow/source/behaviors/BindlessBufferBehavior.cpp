#include "BindlessBufferBehavior.h"

#include <glow/Error.h>

namespace glow {

void * BindlessBufferBehavior::map(const Buffer * buffer, GLenum access) const
{
    void * result = glMapNamedBufferEXT(buffer->id(), access);
    CheckGLError();

    return result;
}

void * BindlessBufferBehavior::mapRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length, GLbitfield access) const
{
    void* result = glMapNamedBufferRangeEXT(buffer->id(), offset, length, access);
    CheckGLError();

    return result;
}

bool BindlessBufferBehavior::unmap(const Buffer * buffer) const
{
    GLboolean success = glUnmapNamedBufferEXT(buffer->id());
    CheckGLError();

    return success == GL_TRUE;
}

void BindlessBufferBehavior::setData(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLenum usage) const
{
    glNamedBufferDataEXT(buffer->id(), size, data, usage);
    CheckGLError();
}

void BindlessBufferBehavior::setSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data) const
{
    glNamedBufferSubDataEXT(buffer->id(), offset, size, data);
    CheckGLError();
}

void BindlessBufferBehavior::setStorage(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLbitfield flags) const
{
    glNamedBufferStorageEXT(buffer->id(), size, data, flags);
    CheckGLError();
}

void BindlessBufferBehavior::copySubData(const glow::Buffer * buffer, glow::Buffer * other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const
{
    glNamedCopyBufferSubDataEXT(buffer->id(), other->id(), readOffset, writeOffset, size);
    CheckGLError();
}

void BindlessBufferBehavior::getParameter(const Buffer * buffer, GLenum pname, GLint * data) const
{
    glGetNamedBufferParameterivEXT(buffer->id(), pname, data);
    CheckGLError();
}

void BindlessBufferBehavior::clearData(const Buffer * buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const
{
    glClearNamedBufferDataEXT(buffer->id(), internalformat, format, type, data);
    CheckGLError();
}

void BindlessBufferBehavior::clearSubData(const Buffer * buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const
{
    glClearNamedBufferSubDataEXT(buffer->id(), internalformat, offset, size, format, type, data);
    CheckGLError();
}

} // namespace glow
