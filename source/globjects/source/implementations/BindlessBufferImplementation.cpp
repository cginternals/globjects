#include "BindlessBufferImplementation.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Buffer.h>

namespace glo {

void * BindlessBufferImplementation::map(const Buffer * buffer, gl::GLenum access) const
{
    return gl::glMapNamedBuffer(buffer->id(), access);
}

void * BindlessBufferImplementation::mapRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length, gl::BufferAccessMask access) const
{
    return gl::glMapNamedBufferRange(buffer->id(), offset, static_cast<gl::GLsizei>(length), access);
}

bool BindlessBufferImplementation::unmap(const Buffer * buffer) const
{
    return gl::GL_TRUE == gl::glUnmapNamedBuffer(buffer->id());
}

void BindlessBufferImplementation::setData(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage) const
{
    gl::glNamedBufferData(buffer->id(), static_cast<gl::GLsizei>(size), data, usage);
}

void BindlessBufferImplementation::setSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid * data) const
{
    gl::glNamedBufferSubData(buffer->id(), offset, static_cast<gl::GLsizei>(size), data);
}

void BindlessBufferImplementation::setStorage(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::MapBufferUsageMask flags) const
{
    gl::glNamedBufferStorage(buffer->id(), static_cast<gl::GLsizei>(size), data, flags);
}

void BindlessBufferImplementation::copySubData(const Buffer * buffer, Buffer * other, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const
{
    gl::glCopyNamedBufferSubData(buffer->id(), other->id(), readOffset, writeOffset, static_cast<gl::GLsizei>(size));
}

void BindlessBufferImplementation::getParameter(const Buffer * buffer, gl::GLenum pname, gl::GLint * data) const
{
    gl::glGetNamedBufferParameteriv(buffer->id(), pname, data);
}

void BindlessBufferImplementation::clearData(const Buffer * buffer, gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data) const
{
    gl::glClearNamedBufferData(buffer->id(), internalformat, format, type, data);
}

void BindlessBufferImplementation::clearSubData(const Buffer * buffer, gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data) const
{
    gl::glClearNamedBufferSubData(buffer->id(), internalformat, offset, static_cast<gl::GLsizei>(size), format, type, data);
}

void BindlessBufferImplementation::flushMappedRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length) const
{
    gl::glFlushMappedNamedBufferRange(buffer->id(), offset, static_cast<gl::GLsizei>(length));
}

} // namespace glo
