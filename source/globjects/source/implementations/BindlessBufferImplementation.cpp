
#include "BindlessBufferImplementation.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Buffer.h>

using namespace gl;

namespace glo 
{

GLuint BindlessBufferImplementation::create() const
{
    GLuint buffer;
    glCreateBuffers(1, &buffer); // create a handle as well as the actual buffer

    return buffer;
}

void BindlessBufferImplementation::destroy(const GLuint id) const
{
    glDeleteBuffers(1, &id);
}

void * BindlessBufferImplementation::map(const Buffer * buffer, GLenum access) const
{
    return glMapNamedBuffer(buffer->id(), access);
}

void * BindlessBufferImplementation::mapRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length, BufferAccessMask access) const
{
    return glMapNamedBufferRange(buffer->id(), offset, static_cast<GLsizei>(length), access);
}

bool BindlessBufferImplementation::unmap(const Buffer * buffer) const
{
    return GL_TRUE == glUnmapNamedBuffer(buffer->id());
}

void BindlessBufferImplementation::setData(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLenum usage) const
{
    glNamedBufferData(buffer->id(), static_cast<GLsizei>(size), data, usage);
}

void BindlessBufferImplementation::setSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data) const
{
    glNamedBufferSubData(buffer->id(), offset, static_cast<GLsizei>(size), data);
}

void BindlessBufferImplementation::setStorage(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, MapBufferUsageMask flags) const
{
    glNamedBufferStorage(buffer->id(), static_cast<GLsizei>(size), data, flags);
}

void BindlessBufferImplementation::copySubData(const Buffer * buffer, Buffer * other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const
{
    glCopyNamedBufferSubData(buffer->id(), other->id(), readOffset, writeOffset, static_cast<GLsizei>(size));
}

void BindlessBufferImplementation::getParameter(const Buffer * buffer, GLenum pname, GLint * data) const
{
    glGetNamedBufferParameteriv(buffer->id(), pname, data);
}

void BindlessBufferImplementation::clearData(const Buffer * buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const
{
    glClearNamedBufferData(buffer->id(), internalformat, format, type, data);
}

void BindlessBufferImplementation::clearSubData(const Buffer * buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const
{
    glClearNamedBufferSubData(buffer->id(), internalformat, offset, static_cast<GLsizei>(size), format, type, data);
}

void BindlessBufferImplementation::flushMappedRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length) const
{
    glFlushMappedNamedBufferRange(buffer->id(), offset, static_cast<GLsizei>(length));
}

} // namespace glo
