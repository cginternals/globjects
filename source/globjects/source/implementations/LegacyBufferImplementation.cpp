
#include "LegacyBufferImplementation.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Buffer.h>

using namespace gl;

namespace glo 
{

GLuint LegacyBufferImplementation::create() const
{
    GLuint buffer;
    glGenBuffers(1, &buffer); // create a handle to a potentially used buffer
    glBindBuffer(s_workingTarget, buffer); // trigger actual buffer creation

    return buffer;
}

void LegacyBufferImplementation::destroy(const GLuint id) const
{
    glDeleteBuffers(1, &id);
}

void * LegacyBufferImplementation::map(const Buffer * buffer, GLenum access) const
{
    buffer->bind(s_workingTarget);

    return glMapBuffer(s_workingTarget, access);
}

void * LegacyBufferImplementation::mapRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length, BufferAccessMask access) const
{
    buffer->bind(s_workingTarget);

    return glMapBufferRange(s_workingTarget, offset, length, access);
}

bool LegacyBufferImplementation::unmap(const Buffer * buffer) const
{
    buffer->bind(s_workingTarget);

    return GL_TRUE == glUnmapBuffer(s_workingTarget);
}

void LegacyBufferImplementation::setData(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLenum usage) const
{
    buffer->bind(s_workingTarget);

    glBufferData(s_workingTarget, size, data, usage);
}

void LegacyBufferImplementation::setSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data) const
{
    buffer->bind(s_workingTarget);

    glBufferSubData(s_workingTarget, offset, size, data);
}

void LegacyBufferImplementation::setStorage(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, MapBufferUsageMask flags) const
{
    buffer->bind(s_workingTarget);

    glBufferStorage(s_workingTarget, size, data, flags);
}

void LegacyBufferImplementation::copySubData(const Buffer * buffer, Buffer * other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const
{
    GLenum readTarget = GL_COPY_READ_BUFFER;
    GLenum writeTarget = GL_COPY_WRITE_BUFFER;

    buffer->bind(readTarget);
    other->bind(writeTarget);

    glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
}

void LegacyBufferImplementation::getParameter(const Buffer * buffer, GLenum pname, GLint * data) const
{
    buffer->bind(s_workingTarget);

    glGetBufferParameteriv(s_workingTarget, pname, data);
}

void LegacyBufferImplementation::clearData(const Buffer * buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const
{
    buffer->bind(s_workingTarget);

    glClearBufferData(s_workingTarget, internalformat, format, type, data);
}

void LegacyBufferImplementation::clearSubData(const Buffer * buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const
{
    buffer->bind(s_workingTarget);

    glClearBufferSubData(s_workingTarget, internalformat, offset, size, format, type, data);
}

void LegacyBufferImplementation::flushMappedRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length) const
{
    buffer->bind(s_workingTarget);

    glFlushMappedBufferRange(s_workingTarget, offset, length);
}

} // namespace glo
