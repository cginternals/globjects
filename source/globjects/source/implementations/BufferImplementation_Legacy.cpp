
#include "BufferImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/base/baselogging.h>
#include <globjects/Buffer.h>


using namespace gl;

namespace globjects 
{

GLenum BufferImplementation_Legacy::s_workingTarget = GL_COPY_WRITE_BUFFER;

GLuint BufferImplementation_Legacy::create() const
{
    GLuint buffer;
    glGenBuffers(1, &buffer); // create a handle to a potentially used buffer
    glBindBuffer(s_workingTarget, buffer); // trigger actual buffer creation

    return buffer;
}

void BufferImplementation_Legacy::destroy(const GLuint id) const
{
    glDeleteBuffers(1, &id);
}

void * BufferImplementation_Legacy::map(const Buffer * buffer, GLenum access) const
{
    buffer->bind(s_workingTarget);

    return glMapBuffer(s_workingTarget, access);
}

void * BufferImplementation_Legacy::mapRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length, BufferAccessMask access) const
{
    buffer->bind(s_workingTarget);

    return glMapBufferRange(s_workingTarget, offset, length, access);
}

bool BufferImplementation_Legacy::unmap(const Buffer * buffer) const
{
    buffer->bind(s_workingTarget);

    return GL_TRUE == glUnmapBuffer(s_workingTarget);
}

void BufferImplementation_Legacy::setData(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLenum usage) const
{
    buffer->bind(s_workingTarget);

    glBufferData(s_workingTarget, size, data, usage);
}

void BufferImplementation_Legacy::setSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data) const
{
    buffer->bind(s_workingTarget);

    glBufferSubData(s_workingTarget, offset, size, data);
}

void BufferImplementation_Legacy::setStorage(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, BufferStorageMask flags) const
{
    buffer->bind(s_workingTarget);

    glBufferStorage(s_workingTarget, size, data, flags);
}

void BufferImplementation_Legacy::copySubData(const Buffer * buffer, Buffer * other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const
{
    GLenum readTarget = GL_COPY_READ_BUFFER;
    GLenum writeTarget = GL_COPY_WRITE_BUFFER;

    buffer->bind(readTarget);
    other->bind(writeTarget);

    glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
}

GLint BufferImplementation_Legacy::getParameter(const Buffer * buffer, GLenum pname) const
{
    buffer->bind(s_workingTarget);

    GLint value = 0;

    glGetBufferParameteriv(s_workingTarget, pname, &value);

    return value;
}

GLint64 BufferImplementation_Legacy::getParameter64(const Buffer * buffer, GLenum pname) const
{
    buffer->bind(s_workingTarget);

    GLint64 value = 0;

    glGetBufferParameteri64v(s_workingTarget, pname, &value);

    return value;
}

void * BufferImplementation_Legacy::getPointer(const Buffer * buffer, GLenum pname) const
{
    buffer->bind(s_workingTarget);

    void * data = nullptr;

    glGetBufferPointerv(s_workingTarget, pname, &data);

    return data;
}

void BufferImplementation_Legacy::clearData(const Buffer * buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const
{
    buffer->bind(s_workingTarget);

    glClearBufferData(s_workingTarget, internalformat, format, type, data);
}

void BufferImplementation_Legacy::clearSubData(const Buffer * buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const
{
    buffer->bind(s_workingTarget);

    glClearBufferSubData(s_workingTarget, internalformat, offset, size, format, type, data);
}

void BufferImplementation_Legacy::flushMappedRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length) const
{
    buffer->bind(s_workingTarget);

    glFlushMappedBufferRange(s_workingTarget, offset, length);
}

void BufferImplementation_Legacy::getBufferSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, GLvoid * data) const
{
    buffer->bind(s_workingTarget);

    glGetBufferSubData(s_workingTarget, offset, size, data);
}

void BufferImplementation_Legacy::invalidateData(const Buffer * /* buffer */) const
{
    critical() << "glInvalidateBufferData requires direct state access";
}

void BufferImplementation_Legacy::invalidateSubData(const Buffer * /* buffer */, GLintptr /* offset */, GLsizeiptr /* length */) const
{
    critical() << "glInvalidateBufferSubData requires direct state access";
}

} // namespace globjects
