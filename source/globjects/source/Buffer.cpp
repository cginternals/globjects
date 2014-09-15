#include <globjects/Buffer.h>

#include <cassert>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>
#include <globjects/ObjectVisitor.h>

#include "registry/ImplementationRegistry.h"

#include "Resource.h"

#include "registry/ImplementationRegistry.h"
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

Buffer * Buffer::fromId(GLuint id)
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

void Buffer::bind(GLenum target) const
{
    glBindBuffer(target, id());
}

void Buffer::unbind(GLenum target)
{
    glBindBuffer(target, 0);
}

void Buffer::unbind(GLenum target, GLuint index)
{
    glBindBufferBase(target, index, 0);
}

const void * Buffer::map() const
{
    return static_cast<const void*>(implementation().map(this, GL_READ_ONLY));
}

void* Buffer::map(GLenum access)
{
    return implementation().map(this, access);
}

void* Buffer::mapRange(GLintptr offset, GLsizeiptr length, BufferAccessMask access)
{
    return implementation().mapRange(this, offset, length, access);
}

bool Buffer::unmap() const
{
    return implementation().unmap(this);
}

void Buffer::flushMappedRange(GLintptr offset, GLsizeiptr length)
{
    implementation().flushMappedRange(this, offset, length);
}

void Buffer::setData(GLsizeiptr size, const GLvoid * data, GLenum usage)
{
    implementation().setData(this, size, data, usage);
}
    
void Buffer::setSubData(GLintptr offset, GLsizeiptr size, const GLvoid * data)
{
    implementation().setSubData(this, offset, size, data);
}

void Buffer::setStorage(GLsizeiptr size, const GLvoid * data, MapBufferUsageMask flags)
{
    implementation().setStorage(this, size, data, flags);
}

GLint Buffer::getParameter(GLenum pname) const
{
    return implementation().getParameter(this, pname);
}

GLint64 Buffer::getParameter64(GLenum pname) const
{
    return implementation().getParameter64(this, pname);
}

void Buffer::bindBase(GLenum target, GLuint index) const
{
    glBindBufferBase(target, index, id());
}

void Buffer::bindRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size) const
{
    glBindBufferRange(target, index, id(), offset, size);
}

void Buffer::copySubData(Buffer * buffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const
{
    assert(buffer != nullptr);

    implementation().copySubData(this, buffer, readOffset, writeOffset, size);
}

void Buffer::copySubData(Buffer * buffer, GLsizeiptr size) const
{
	copySubData(buffer, 0, 0, size);
}

void Buffer::copyData(Buffer * buffer, GLsizeiptr size, GLenum usage) const
{
    assert(buffer != nullptr);

    buffer->setData(static_cast<GLsizei>(size), nullptr, usage);
	copySubData(buffer, 0, 0, size);
}

void Buffer::clearData(GLenum internalformat, GLenum format, GLenum type, const void * data)
{
    implementation().clearData(this, internalformat, format, type, data);
}

void Buffer::clearSubData(GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data)
{
    implementation().clearSubData(this, internalformat, offset, size, format, type, data);
}

const void * Buffer::getPointer() const
{
    return getPointer(gl::GL_BUFFER_MAP_POINTER);
}

void * Buffer::getPointer()
{
    return getPointer(gl::GL_BUFFER_MAP_POINTER);
}

const void * Buffer::getPointer(gl::GLenum pname) const
{
    return implementation().getPointer(this, pname);
}

void * Buffer::getPointer(gl::GLenum pname)
{
    return implementation().getPointer(this, pname);
}

void Buffer::getSubData(gl::GLintptr offset, gl::GLsizeiptr size, void * data) const
{
    implementation().getBufferSubData(this, offset, size, data);
}

gl::GLenum Buffer::objectType() const
{
    return gl::GL_BUFFER;
}

} // namespace globjects
