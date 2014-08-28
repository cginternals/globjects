
#include "BindlessEXTBufferImplementation.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Buffer.h>


using namespace gl;

namespace glo 
{

GLuint BindlessEXTBufferImplementation::create() const
{
    // ToDo: use legacy impl. (singleton impls)

    GLuint buffer;
    glGenBuffers(1, &buffer); // create a handle to a potentially used buffer
    glBindBuffer(s_workingTarget, buffer); // trigger actual buffer creation

    return buffer;
}

void BindlessEXTBufferImplementation::destroy(const GLuint id) const
{
    glDeleteBuffers(1, &id);
}

void * BindlessEXTBufferImplementation::map(const Buffer * buffer, GLenum access) const
{
    return glMapNamedBufferEXT(buffer->id(), access);
}

void * BindlessEXTBufferImplementation::mapRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length, BufferAccessMask access) const
{
    return glMapNamedBufferRangeEXT(buffer->id(), offset, length, access);
}

bool BindlessEXTBufferImplementation::unmap(const Buffer * buffer) const
{
    return GL_TRUE == glUnmapNamedBufferEXT(buffer->id());
}

void BindlessEXTBufferImplementation::setData(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, GLenum usage) const
{
    glNamedBufferDataEXT(buffer->id(), size, data, usage);
}

void BindlessEXTBufferImplementation::setSubData(const Buffer * buffer, GLintptr offset, GLsizeiptr size, const GLvoid * data) const
{
    glNamedBufferSubDataEXT(buffer->id(), offset, size, data);
}

void BindlessEXTBufferImplementation::setStorage(const Buffer * buffer, GLsizeiptr size, const GLvoid * data, MapBufferUsageMask flags) const
{
    glNamedBufferStorageEXT(buffer->id(), size, data, flags);
}

void BindlessEXTBufferImplementation::copySubData(const Buffer * buffer, Buffer * other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const
{
    glNamedCopyBufferSubDataEXT(buffer->id(), other->id(), readOffset, writeOffset, size);
}

void BindlessEXTBufferImplementation::getParameter(const Buffer * buffer, GLenum pname, GLint * data) const
{
    glGetNamedBufferParameterivEXT(buffer->id(), pname, data);
}

void BindlessEXTBufferImplementation::clearData(const Buffer * buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) const
{
    glClearNamedBufferDataEXT(buffer->id(), internalformat, format, type, data);
}

void BindlessEXTBufferImplementation::clearSubData(const Buffer * buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) const
{
    glClearNamedBufferSubDataEXT(buffer->id(), internalformat, offset, size, format, type, data);
}

void BindlessEXTBufferImplementation::flushMappedRange(const Buffer * buffer, GLintptr offset, GLsizeiptr length) const
{
    glFlushMappedNamedBufferRangeEXT(buffer->id(), offset, length);
}

} // namespace glo
