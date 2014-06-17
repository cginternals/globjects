#include "BindlessBufferImplementation.h"

#include <glbinding/functions.h>

#include <glow/Buffer.h>

namespace glow {

void * BindlessBufferImplementation::map(const Buffer * buffer, gl::GLenum access) const
{
    return gl::glMapNamedBufferEXT(buffer->id(), access);
}

void * BindlessBufferImplementation::mapRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length, gl::GLbitfield access) const
{
    return gl::glMapNamedBufferRangeEXT(buffer->id(), offset, length, access);
}

bool BindlessBufferImplementation::unmap(const Buffer * buffer) const
{
    return gl::GL_TRUE == gl::glUnmapNamedBufferEXT(buffer->id());
}

void BindlessBufferImplementation::setData(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage) const
{
    gl::glNamedBufferDataEXT(buffer->id(), size, data, usage);
}

void BindlessBufferImplementation::setSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid * data) const
{
    gl::glNamedBufferSubDataEXT(buffer->id(), offset, size, data);
}

void BindlessBufferImplementation::setStorage(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLbitfield flags) const
{
    gl::glNamedBufferStorageEXT(buffer->id(), size, data, flags);
}

void BindlessBufferImplementation::copySubData(const Buffer * buffer, Buffer * other, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const
{
    gl::glNamedCopyBufferSubDataEXT(buffer->id(), other->id(), readOffset, writeOffset, size);
}

void BindlessBufferImplementation::getParameter(const Buffer * buffer, gl::GLenum pname, gl::GLint * data) const
{
    gl::glGetNamedBufferParameterivEXT(buffer->id(), pname, data);
}

void BindlessBufferImplementation::clearData(const Buffer * buffer, gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data) const
{
    gl::glClearNamedBufferDataEXT(buffer->id(), internalformat, format, type, data);
}

void BindlessBufferImplementation::clearSubData(const Buffer * buffer, gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data) const
{
    gl::glClearNamedBufferSubDataEXT(buffer->id(), internalformat, offset, size, format, type, data);
}

void BindlessBufferImplementation::flushMappedRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length) const
{
    gl::glFlushMappedNamedBufferRangeEXT(buffer->id(), offset, length);
}

} // namespace glow
