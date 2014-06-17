#include "LegacyBufferImplementation.h"

#include <glbinding/functions.h>

#include <glow/Buffer.h>

namespace glow {

gl::GLenum LegacyBufferImplementation::s_workingTarget = gl::GL_COPY_WRITE_BUFFER;

void * LegacyBufferImplementation::map(const Buffer * buffer, gl::GLenum access) const
{
    buffer->bind(s_workingTarget);

    return gl::glMapBuffer(s_workingTarget, access);
}

void * LegacyBufferImplementation::mapRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length, gl::GLbitfield access) const
{
    buffer->bind(s_workingTarget);

    return gl::glMapBufferRange(s_workingTarget, offset, length, access);
}

bool LegacyBufferImplementation::unmap(const Buffer * buffer) const
{
    buffer->bind(s_workingTarget);

    return gl::GL_TRUE == gl::glUnmapBuffer(s_workingTarget);
}

void LegacyBufferImplementation::setData(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage) const
{
    buffer->bind(s_workingTarget);

    gl::glBufferData(s_workingTarget, size, data, usage);
}

void LegacyBufferImplementation::setSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid * data) const
{
    buffer->bind(s_workingTarget);

    gl::glBufferSubData(s_workingTarget, offset, size, data);
}

void LegacyBufferImplementation::setStorage(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLbitfield flags) const
{
    buffer->bind(s_workingTarget);

    gl::glBufferStorage(s_workingTarget, size, data, flags);
}

void LegacyBufferImplementation::copySubData(const Buffer * buffer, Buffer * other, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const
{
    gl::GLenum readTarget = gl::GL_COPY_READ_BUFFER;
    gl::GLenum writeTarget = gl::GL_COPY_WRITE_BUFFER;

    buffer->bind(readTarget);
    other->bind(writeTarget);

    gl::glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
}

void LegacyBufferImplementation::getParameter(const Buffer * buffer, gl::GLenum pname, gl::GLint * data) const
{
    buffer->bind(s_workingTarget);

    gl::glGetBufferParameteriv(s_workingTarget, pname, data);
}

void LegacyBufferImplementation::clearData(const Buffer * buffer, gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data) const
{
    buffer->bind(s_workingTarget);

    gl::glClearBufferData(s_workingTarget, internalformat, format, type, data);
}

void LegacyBufferImplementation::clearSubData(const Buffer * buffer, gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data) const
{
    buffer->bind(s_workingTarget);

    gl::glClearBufferSubData(s_workingTarget, internalformat, offset, size, format, type, data);
}

void LegacyBufferImplementation::flushMappedRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length) const
{
    buffer->bind(s_workingTarget);

    gl::glFlushMappedBufferRange(s_workingTarget, offset, length);
}

} // namespace glow
