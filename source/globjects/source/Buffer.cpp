#include <globjects/Buffer.h>

#include <cassert>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>
#include <globjects/ObjectVisitor.h>

#include "registry/ImplementationRegistry.h"

#include "Resource.h"

#include "implementations/BufferImplementation_Legacy.h"


using namespace gl;

namespace 
{

const globjects::AbstractBufferImplementation & implementation()
{
    return globjects::ImplementationRegistry::current().bufferImplementation();
}

}

namespace globjects
{

void Buffer::hintBindlessImplementation(BindlessImplementation impl)
{
    ImplementationRegistry::current().initialize(impl);
}

void Buffer::setWorkingTarget(GLenum target)
{
    BufferImplementation_Legacy::s_workingTarget = target;
}


Buffer::Buffer()
: Buffer(new BufferResource)
{
}

Buffer::Buffer(IDResource * resource)
: Object(resource)
{
}

Buffer * Buffer::fromId(const GLuint id)
{
    return new Buffer(new ExternalResource(id));
}

Buffer::~Buffer()
{
}

void Buffer::accept(ObjectVisitor& visitor)
{
	visitor.visitBuffer(this);
}

void Buffer::bind(const GLenum target) const
{
    glBindBuffer(target, id());
}

void Buffer::unbind(const GLenum target)
{
    glBindBuffer(target, 0);
}

void Buffer::unbind(const GLenum target, const GLuint index)
{
    glBindBufferBase(target, index, 0);
}

const void * Buffer::map() const
{
    return static_cast<const void*>(implementation().map(this, GL_READ_ONLY));
}

void* Buffer::map(const GLenum access)
{
    return implementation().map(this, access);
}

void* Buffer::mapRange(const GLintptr offset, const GLsizeiptr length, const BufferAccessMask access)
{
    return implementation().mapRange(this, offset, length, access);
}

bool Buffer::unmap() const
{
    return implementation().unmap(this);
}

void Buffer::flushMappedRange(const GLintptr offset, const GLsizeiptr length)
{
    implementation().flushMappedRange(this, offset, length);
}

void Buffer::setData(const GLsizeiptr size, const GLvoid * data, const GLenum usage)
{
    implementation().setData(this, size, data, usage);
}
    
void Buffer::setSubData(const GLintptr offset, const GLsizeiptr size, const GLvoid * data)
{
    implementation().setSubData(this, offset, size, data);
}

void Buffer::setStorage(const GLsizeiptr size, const GLvoid * data, const BufferStorageMask flags)
{
    implementation().setStorage(this, size, data, flags);
}

GLint Buffer::getParameter(const GLenum pname) const
{
    return implementation().getParameter(this, pname);
}

GLint64 Buffer::getParameter64(const GLenum pname) const
{
    return implementation().getParameter64(this, pname);
}

void Buffer::bindBase(const GLenum target, const GLuint index) const
{
    glBindBufferBase(target, index, id());
}

void Buffer::bindRange(const GLenum target, const GLuint index, const GLintptr offset, const GLsizeiptr size) const
{
    glBindBufferRange(target, index, id(), offset, size);
}

void Buffer::copySubData(Buffer * buffer, const GLintptr readOffset, const GLintptr writeOffset, const GLsizeiptr size) const
{
    assert(buffer != nullptr);

    implementation().copySubData(this, buffer, readOffset, writeOffset, size);
}

void Buffer::copySubData(Buffer * buffer, const GLsizeiptr size) const
{
	copySubData(buffer, 0, 0, size);
}

void Buffer::copyData(Buffer * buffer, const GLsizeiptr size, const GLenum usage) const
{
    assert(buffer != nullptr);

    buffer->setData(static_cast<GLsizei>(size), nullptr, usage);
	copySubData(buffer, 0, 0, size);
}

void Buffer::clearData(const GLenum internalformat, const GLenum format, const GLenum type, const void * data)
{
    implementation().clearData(this, internalformat, format, type, data);
}

void Buffer::clearSubData(const GLenum internalformat, const GLintptr offset, const GLsizeiptr size, const GLenum format, const GLenum type, const void * data)
{
    implementation().clearSubData(this, internalformat, offset, size, format, type, data);
}

const void * Buffer::getPointer() const
{
    return getPointer(GL_BUFFER_MAP_POINTER);
}

void * Buffer::getPointer()
{
    return getPointer(GL_BUFFER_MAP_POINTER);
}

const void * Buffer::getPointer(const GLenum pname) const
{
    return implementation().getPointer(this, pname);
}

void * Buffer::getPointer(const GLenum pname)
{
    return implementation().getPointer(this, pname);
}

void Buffer::getSubData(const GLintptr offset, const GLsizeiptr size, void * data) const
{
    implementation().getBufferSubData(this, offset, size, data);
}

void Buffer::invalidateData() const
{
    implementation().invalidateData(this);
}

void Buffer::invalidateSubData(const GLintptr offset, const GLsizeiptr length) const
{
    implementation().invalidateSubData(this, offset, length);
}

GLenum Buffer::objectType() const
{
    return GL_BUFFER;
}

} // namespace globjects
