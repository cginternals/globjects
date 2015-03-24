
#include "BufferImplementation_DirectStateAccessARB.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Buffer.h>

#include "BufferImplementation_Legacy.h"


using namespace gl;

namespace globjects 
{

GLuint BufferImplementation_DirectStateAccessARB::create() const
{
    GLuint buffer;
    glCreateBuffers(1, &buffer); // create a handle as well as the actual buffer

    return buffer;
}

void BufferImplementation_DirectStateAccessARB::destroy(const GLuint id) const
{
    BufferImplementation_Legacy::instance()->destroy(id);
}

void * BufferImplementation_DirectStateAccessARB::map(const Buffer * buffer, GLenum access) const
{
    return glMapNamedBuffer(buffer->id(), access);
}

void * BufferImplementation_DirectStateAccessARB::mapRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length, BufferAccessMask access) const
{
    return glMapNamedBufferRange(buffer->id(), offset, static_cast<GLsizei>(length), access);
}

bool BufferImplementation_DirectStateAccessARB::unmap(const Buffer * buffer) const
{
    return GL_TRUE == glUnmapNamedBuffer(buffer->id());
}

void BufferImplementation_DirectStateAccessARB::setData(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLenum usage) const
{
    glNamedBufferData(buffer->id(), static_cast<GLsizei>(size), data, usage);
}

void BufferImplementation_DirectStateAccessARB::setSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data) const
{
    glNamedBufferSubData(buffer->id(), offset, static_cast<GLsizei>(size), data);
}

void BufferImplementation_DirectStateAccessARB::setStorage(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, BufferStorageMask flags) const
{
    glNamedBufferStorage(buffer->id(), static_cast<GLsizei>(size), data, flags);
}

void BufferImplementation_DirectStateAccessARB::copySubData(const Buffer * buffer, Buffer * other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const
{
    glCopyNamedBufferSubData(buffer->id(), other->id(), readOffset, writeOffset, static_cast<GLsizei>(size));
}

GLint BufferImplementation_DirectStateAccessARB::getParameter(const Buffer * buffer, GLenum pname) const
{
    GLint value = 0;

    glGetNamedBufferParameteriv(buffer->id(), pname, &value);

    return value;
}

GLint64 BufferImplementation_DirectStateAccessARB::getParameter64(const Buffer * buffer, GLenum pname) const
{
    GLint64 value = 0;

    glGetNamedBufferParameteri64v(buffer->id(), pname, &value);

    return value;
}

void * BufferImplementation_DirectStateAccessARB::getPointer(const Buffer * buffer, GLenum pname) const
{
    void * data = nullptr;

    glGetNamedBufferPointerv(buffer->id(), pname, &data);

    return data;
}

void BufferImplementation_DirectStateAccessARB::clearData(const Buffer * buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const
{
    glClearNamedBufferData(buffer->id(), internalformat, format, type, data);
}

void BufferImplementation_DirectStateAccessARB::clearSubData(const Buffer * buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const
{
    glClearNamedBufferSubData(buffer->id(), internalformat, offset, static_cast<GLsizei>(size), format, type, data);
}

void BufferImplementation_DirectStateAccessARB::flushMappedRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length) const
{
    glFlushMappedNamedBufferRange(buffer->id(), offset, static_cast<GLsizei>(length));
}

void BufferImplementation_DirectStateAccessARB::getBufferSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, GLvoid * data) const
{
    glGetNamedBufferSubData(buffer->id(), offset, static_cast<GLsizei>(size), data);
}

void BufferImplementation_DirectStateAccessARB::invalidateData(const Buffer * buffer) const
{
    glInvalidateBufferData(buffer->id());
}

void BufferImplementation_DirectStateAccessARB::invalidateSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr length) const
{
    glInvalidateBufferSubData(buffer->id(), offset, length);
}

} // namespace globjects
