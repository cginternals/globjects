#include <glow/Buffer.h>

#include <cassert>

#include <glbinding/gl/functions.h>

#include <glow/glow.h>
#include <glow/ObjectVisitor.h>

#include "registry/ImplementationRegistry.h"

#include "Resource.h"

#include "implementations/AbstractBufferImplementation.h"
#include "implementations/LegacyBufferImplementation.h"

namespace {

const glow::AbstractBufferImplementation & implementation()
{
    return glow::ImplementationRegistry::current().bufferImplementation();
}

}

namespace glow
{

void Buffer::setWorkingTarget(gl::GLenum target)
{
    LegacyBufferImplementation::s_workingTarget = target;
}

Buffer::Buffer()
: Object(new BufferResource)
{
}

Buffer::Buffer(IDResource * resource)
: Object(resource)
{
}

Buffer * Buffer::fromId(gl::GLuint id)
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

void Buffer::bind(gl::GLenum target) const
{
    gl::glBindBuffer(target, id());
}

void Buffer::unbind(gl::GLenum target)
{
    gl::glBindBuffer(target, 0);
}

void Buffer::unbind(gl::GLenum target, gl::GLuint index)
{
    gl::glBindBufferBase(target, index, 0);
}

const void * Buffer::map() const
{
    return static_cast<const void*>(implementation().map(this, gl::GL_READ_ONLY));
}

void* Buffer::map(gl::GLenum access)
{
    return implementation().map(this, access);
}

void* Buffer::mapRange(gl::GLintptr offset, gl::GLsizeiptr length, gl::BufferAccessMask access)
{
    return implementation().mapRange(this, offset, length, access);
}

bool Buffer::unmap() const
{
    return implementation().unmap(this);
}

void Buffer::flushMappedRange(gl::GLintptr offset, gl::GLsizeiptr length)
{
    implementation().flushMappedRange(this, offset, length);
}

void Buffer::setData(gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLenum usage)
{
    implementation().setData(this, size, data, usage);
}
    
void Buffer::setSubData(gl::GLintptr offset, gl::GLsizeiptr size, const gl::GLvoid * data)
{
    implementation().setSubData(this, offset, size, data);
}

void Buffer::setStorage(gl::GLsizeiptr size, const gl::GLvoid * data, gl::MapBufferUsageMask flags)
{
    implementation().setStorage(this, size, data, flags);
}

gl::GLint Buffer::getParameter(gl::GLenum pname) const
{
    gl::GLint value = 0;

    implementation().getParameter(this, pname, &value);

    return value;
}

void Buffer::bindBase(gl::GLenum target, gl::GLuint index) const
{
    gl::glBindBufferBase(target, index, id());
}

void Buffer::bindRange(gl::GLenum target, gl::GLuint index, gl::GLintptr offset, gl::GLsizeiptr size) const
{
    gl::glBindBufferRange(target, index, id(), offset, size);
}

void Buffer::copySubData(Buffer * buffer, gl::GLintptr readOffset, gl::GLintptr writeOffset, gl::GLsizeiptr size) const
{
    assert(buffer != nullptr);

    implementation().copySubData(this, buffer, readOffset, writeOffset, size);
}

void Buffer::copySubData(Buffer * buffer, gl::GLsizeiptr size) const
{
	copySubData(buffer, 0, 0, size);
}

void Buffer::copyData(Buffer * buffer, gl::GLsizeiptr size, gl::GLenum usage) const
{
    assert(buffer != nullptr);

    buffer->setData(static_cast<gl::GLsizei>(size), nullptr, usage);
	copySubData(buffer, 0, 0, size);
}

void Buffer::clearData(gl::GLenum internalformat, gl::GLenum format, gl::GLenum type, const void * data)
{
    implementation().clearData(this, internalformat, format, type, data);
}

void Buffer::clearSubData(gl::GLenum internalformat, gl::GLintptr offset, gl::GLsizeiptr size, gl::GLenum format, gl::GLenum type, const void * data)
{
    implementation().clearSubData(this, internalformat, offset, size, format, type, data);
}

} // namespace glow
