#include "LegacyBufferImplementation.h"

#include <glow/Error.h>

namespace glow {

GLenum LegacyBufferImplementation::s_workingTarget = GL_COPY_WRITE_BUFFER;

void * LegacyBufferImplementation::map(const Buffer * buffer, GLenum access) const
{
    buffer->bind(s_workingTarget);

    void * result = glMapBuffer(s_workingTarget, access);
    CheckGLError();

    return result;
}

void * LegacyBufferImplementation::mapRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length, GLbitfield access) const
{
    buffer->bind(s_workingTarget);

    void * result = glMapBufferRange(s_workingTarget, offset, length, access);
    CheckGLError();

    return result;
}

bool LegacyBufferImplementation::unmap(const Buffer * buffer) const
{
    buffer->bind(s_workingTarget);

    GLboolean success = glUnmapBuffer(s_workingTarget);
    CheckGLError();

    return success == GL_TRUE;
}

void LegacyBufferImplementation::setData(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLenum usage) const
{
    buffer->bind(s_workingTarget);

    glBufferData(s_workingTarget, size, data, usage);
    CheckGLError();
}

void LegacyBufferImplementation::setSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data) const
{
    buffer->bind(s_workingTarget);

    glBufferSubData(s_workingTarget, offset, size, data);
    CheckGLError();
}

void LegacyBufferImplementation::setStorage(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLbitfield flags) const
{
    buffer->bind(s_workingTarget);

    glBufferStorage(s_workingTarget, size, data, flags);
    CheckGLError();
}

void LegacyBufferImplementation::copySubData(const Buffer * buffer, Buffer * other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const
{
    GLenum readTarget = GL_COPY_READ_BUFFER;
    GLenum writeTarget = GL_COPY_WRITE_BUFFER;

    buffer->bind(readTarget);
    other->bind(writeTarget);

    glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
    CheckGLError();
}

void LegacyBufferImplementation::getParameter(const Buffer * buffer, GLenum pname, GLint * data) const
{
    buffer->bind(s_workingTarget);

    glGetBufferParameteriv(s_workingTarget, pname, data);
    CheckGLError();
}

void LegacyBufferImplementation::clearData(const Buffer * buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const
{
    buffer->bind(s_workingTarget);

    glClearBufferData(s_workingTarget, internalformat, format, type, data);
    CheckGLError();
}

void LegacyBufferImplementation::clearSubData(const Buffer * buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const
{
    buffer->bind(s_workingTarget);

    glClearBufferSubData(s_workingTarget, internalformat, offset, size, format, type, data);
    CheckGLError();
}

} // namespace glow
