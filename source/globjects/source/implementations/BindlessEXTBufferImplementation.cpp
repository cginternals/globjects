#include "BindlessEXTBufferImplementation.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Buffer.h>

namespace glo {

void * BindlessEXTBufferImplementation::map(const Buffer * buffer, gl::GLenum access) const
{
    return gl::glMapNamedBufferEXT(buffer->id(), access);
}

void * BindlessEXTBufferImplementation::mapRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length, gl::BufferAccessMask access) const
{
    return gl::glMapNamedBufferRangeEXT(buffer->id(), offset, length, access);
}

bool BindlessEXTBufferImplementation::unmap(const Buffer * buffer) const
{
    return gl::GL_TRUE == gl::glUnmapNamedBufferEXT(buffer->id());
}

void BindlessEXTBufferImplementation::setData(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage) const
{
    gl::glNamedBufferDataEXT(buffer->id(), size, data, usage);
}

void BindlessEXTBufferImplementation::setSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid * data) const
{
    gl::glNamedBufferSubDataEXT(buffer->id(), offset, size, data);
}

void BindlessEXTBufferImplementation::setStorage(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::MapBufferUsageMask flags) const
{
    gl::glNamedBufferStorageEXT(buffer->id(), size, data, flags);
}

void BindlessEXTBufferImplementation::copySubData(const Buffer * buffer, Buffer * other, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const
{
    gl::glNamedCopyBufferSubDataEXT(buffer->id(), other->id(), readOffset, writeOffset, size);
}

void BindlessEXTBufferImplementation::getParameter(const Buffer * buffer, gl::GLenum pname, gl::GLint * data) const
{
    gl::glGetNamedBufferParameterivEXT(buffer->id(), pname, data);
}

void BindlessEXTBufferImplementation::clearData(const Buffer * buffer, gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data) const
{
    gl::glClearNamedBufferDataEXT(buffer->id(), internalformat, format, type, data);
}

void BindlessEXTBufferImplementation::clearSubData(const Buffer * buffer, gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data) const
{
    gl::glClearNamedBufferSubDataEXT(buffer->id(), internalformat, offset, size, format, type, data);
}

void BindlessEXTBufferImplementation::flushMappedRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length) const
{
    gl::glFlushMappedNamedBufferRangeEXT(buffer->id(), offset, length);
}

} // namespace glo
