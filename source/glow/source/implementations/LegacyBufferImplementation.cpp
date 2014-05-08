#include "LegacyBufferImplementation.h"

#include <glbinding/functions.h>

#include <glow/Error.h>

namespace glow {

gl::GLenum LegacyBufferImplementation::s_workingTarget = gl::COPY_WRITE_BUFFER;

void * LegacyBufferImplementation::map(const Buffer * buffer, gl::GLenum access) const
{
    buffer->bind(s_workingTarget);

    void * result = gl::MapBuffer(s_workingTarget, access);
    CheckGLError();

    return result;
}

void * LegacyBufferImplementation::mapRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length, gl::GLbitfield access) const
{
    buffer->bind(s_workingTarget);

    void * result = gl::MapBufferRange(s_workingTarget, offset, length, access);
    CheckGLError();

    return result;
}

bool LegacyBufferImplementation::unmap(const Buffer * buffer) const
{
    buffer->bind(s_workingTarget);

    gl::GLboolean success = gl::UnmapBuffer(s_workingTarget);
    CheckGLError();

    return success == gl::TRUE;
}

void LegacyBufferImplementation::setData(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage) const
{
    buffer->bind(s_workingTarget);

    gl::BufferData(s_workingTarget, size, data, usage);
    CheckGLError();
}

void LegacyBufferImplementation::setSubData(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid * data) const
{
    buffer->bind(s_workingTarget);

    gl::BufferSubData(s_workingTarget, offset, size, data);
    CheckGLError();
}

void LegacyBufferImplementation::setStorage(const Buffer * buffer, gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLbitfield flags) const
{
    buffer->bind(s_workingTarget);

    gl::BufferStorage(s_workingTarget, size, data, flags);
    CheckGLError();
}

void LegacyBufferImplementation::copySubData(const Buffer * buffer, Buffer * other, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const
{
    gl::GLenum readTarget = gl::COPY_READ_BUFFER;
    gl::GLenum writeTarget = gl::COPY_WRITE_BUFFER;

    buffer->bind(readTarget);
    other->bind(writeTarget);

    gl::CopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
    CheckGLError();
}

void LegacyBufferImplementation::getParameter(const Buffer * buffer, gl::GLenum pname, gl::GLint * data) const
{
    buffer->bind(s_workingTarget);

    gl::GetBufferParameteriv(s_workingTarget, pname, data);
    CheckGLError();
}

void LegacyBufferImplementation::clearData(const Buffer * buffer, gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data) const
{
    buffer->bind(s_workingTarget);

    gl::ClearBufferData(s_workingTarget, internalformat, format, type, data);
    CheckGLError();
}

void LegacyBufferImplementation::clearSubData(const Buffer * buffer, gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data) const
{
    buffer->bind(s_workingTarget);

    gl::ClearBufferSubData(s_workingTarget, internalformat, offset, size, format, type, data);
    CheckGLError();
}

void LegacyBufferImplementation::flushMappedRange(const Buffer * buffer, gl::GLintptr offset, gl::GLsizeiptr length) const
{
    buffer->bind(s_workingTarget);

    gl::FlushMappedBufferRange(s_workingTarget, offset, length);
    CheckGLError();
}

} // namespace glow
