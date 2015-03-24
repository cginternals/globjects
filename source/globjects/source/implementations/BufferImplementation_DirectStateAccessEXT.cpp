
#include "BufferImplementation_DirectStateAccessEXT.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Buffer.h>

#include "BufferImplementation_Legacy.h"


using namespace gl;

namespace globjects 
{

GLuint BufferImplementation_DirectStateAccessEXT::create() const
{
    return BufferImplementation_Legacy::instance()->create();
}

void BufferImplementation_DirectStateAccessEXT::destroy(const GLuint id) const
{
    BufferImplementation_Legacy::instance()->destroy(id);
}

void * BufferImplementation_DirectStateAccessEXT::map(const Buffer * buffer, GLenum access) const
{
    return glMapNamedBufferEXT(buffer->id(), access);
}

void * BufferImplementation_DirectStateAccessEXT::mapRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length, BufferAccessMask access) const
{
    return glMapNamedBufferRangeEXT(buffer->id(), offset, length, access);
}

bool BufferImplementation_DirectStateAccessEXT::unmap(const Buffer * buffer) const
{
    return GL_TRUE == glUnmapNamedBufferEXT(buffer->id());
}

void BufferImplementation_DirectStateAccessEXT::setData(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLenum usage) const
{
    glNamedBufferDataEXT(buffer->id(), size, data, usage);
}

void BufferImplementation_DirectStateAccessEXT::setSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data) const
{
    glNamedBufferSubDataEXT(buffer->id(), offset, size, data);
}

void BufferImplementation_DirectStateAccessEXT::setStorage(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, BufferStorageMask flags) const
{
    glNamedBufferStorageEXT(buffer->id(), size, data, flags);
}

void BufferImplementation_DirectStateAccessEXT::copySubData(const Buffer * buffer, Buffer * other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const
{
    glNamedCopyBufferSubDataEXT(buffer->id(), other->id(), readOffset, writeOffset, size);
}

GLint BufferImplementation_DirectStateAccessEXT::getParameter(const Buffer * buffer, GLenum pname) const
{
    GLint value = 0;

    glGetNamedBufferParameterivEXT(buffer->id(), pname, &value);

    return value;
}

GLint64 BufferImplementation_DirectStateAccessEXT::getParameter64(const Buffer * buffer, GLenum pname) const
{
    return BufferImplementation_Legacy::instance()->getParameter64(buffer, pname);
}

void * BufferImplementation_DirectStateAccessEXT::getPointer(const Buffer * buffer, GLenum pname) const
{
    void * data = nullptr;

    glGetNamedBufferPointervEXT(buffer->id(), pname, &data);

    return data;
}

void BufferImplementation_DirectStateAccessEXT::clearData(const Buffer * buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const
{
    glClearNamedBufferDataEXT(buffer->id(), internalformat, format, type, data);
}

void BufferImplementation_DirectStateAccessEXT::clearSubData(const Buffer * buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const
{
    glClearNamedBufferSubDataEXT(buffer->id(), internalformat, offset, size, format, type, data);
}

void BufferImplementation_DirectStateAccessEXT::flushMappedRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length) const
{
    glFlushMappedNamedBufferRangeEXT(buffer->id(), offset, length);
}

void BufferImplementation_DirectStateAccessEXT::getBufferSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, GLvoid * data) const
{
    glGetNamedBufferSubDataEXT(buffer->id(), offset, size, data);
}

void BufferImplementation_DirectStateAccessEXT::invalidateData(const Buffer * buffer) const
{
    glInvalidateBufferData(buffer->id());
}

void BufferImplementation_DirectStateAccessEXT::invalidateSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr length) const
{
    glInvalidateBufferSubData(buffer->id(), offset, length);
}

} // namespace globjects
