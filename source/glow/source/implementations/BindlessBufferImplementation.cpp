#include "BindlessBufferImplementation.h"

#include <glbinding/functions.h>

#include <glow/Buffer.h>

namespace glow {

void * BindlessBufferImplementation::map(const Buffer * buffer, gl::GLenum access) const
{
    return gl::MapNamedBufferEXT(buffer->id(), access);
}

void * BindlessBufferImplementation::mapRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length, gl::GLbitfield access) const
{
    return gl::MapNamedBufferRangeEXT(buffer->id(), offset, length, access);
}

bool BindlessBufferImplementation::unmap(const Buffer * buffer) const
{
    return gl::TRUE_ == gl::UnmapNamedBufferEXT(buffer->id());
}

void BindlessBufferImplementation::setData(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage) const
{
    gl::NamedBufferDataEXT(buffer->id(), size, data, usage);
}

void BindlessBufferImplementation::setSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid * data) const
{
    gl::NamedBufferSubDataEXT(buffer->id(), offset, size, data);
}

void BindlessBufferImplementation::setStorage(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLbitfield flags) const
{
    gl::NamedBufferStorageEXT(buffer->id(), size, data, flags);
}

void BindlessBufferImplementation::copySubData(const Buffer * buffer, Buffer * other, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const
{
    gl::NamedCopyBufferSubDataEXT(buffer->id(), other->id(), readOffset, writeOffset, size);
}

void BindlessBufferImplementation::getParameter(const Buffer * buffer, gl::GLenum pname, gl::GLint * data) const
{
    gl::GetNamedBufferParameterivEXT(buffer->id(), pname, data);
}

void BindlessBufferImplementation::clearData(const Buffer * buffer, gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data) const
{
    gl::ClearNamedBufferDataEXT(buffer->id(), internalformat, format, type, data);
}

void BindlessBufferImplementation::clearSubData(const Buffer * buffer, gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data) const
{
    gl::ClearNamedBufferSubDataEXT(buffer->id(), internalformat, offset, size, format, type, data);
}

void BindlessBufferImplementation::flushMappedRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length) const
{
    gl::FlushMappedNamedBufferRangeEXT(buffer->id(), offset, length);
}

} // namespace glow
