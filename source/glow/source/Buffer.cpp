#include <glow/Buffer.h>

#include <cassert>

#include <glbinding/functions.h>

#include <glow/glow.h>
#include <glow/ObjectVisitor.h>

#include "registry/ImplementationRegistry.h"

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
: Object(genBuffer())
{
}

Buffer::Buffer(gl::GLuint id, bool takeOwnership)
: Object(id, takeOwnership)
{
}

Buffer * Buffer::fromId(gl::GLuint id, bool takeOwnership)
{
    return new Buffer(id, takeOwnership);
}

gl::GLuint Buffer::genBuffer()
{
    gl::GLuint id = 0;

    gl::GenBuffers(1, &id);

	return id;
}

Buffer::~Buffer()
{
	if (ownsGLObject())
	{
        gl::DeleteBuffers(1, &m_id);
	}
}

void Buffer::accept(ObjectVisitor& visitor)
{
	visitor.visitBuffer(this);
}

void Buffer::bind(gl::GLenum target) const
{
    gl::BindBuffer(target, m_id);
}

void Buffer::unbind(gl::GLenum target)
{
    gl::BindBuffer(target, 0);
}

void Buffer::unbind(gl::GLenum target, gl::GLuint index)
{
    gl::BindBufferBase(target, index, 0);
}

const void * Buffer::map() const
{
    return static_cast<const void*>(implementation().map(this, gl::READ_ONLY));
}

void* Buffer::map(gl::GLenum access)
{
    return implementation().map(this, access);
}

void* Buffer::mapRange(gl::GLintptr offset, gl::GLsizeiptr length, gl::GLbitfield access)
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

void Buffer::setStorage(gl::GLsizeiptr size, const gl::GLvoid * data, gl::GLbitfield flags)
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
    gl::BindBufferBase(target, index, m_id);
}

void Buffer::bindRange(gl::GLenum target, gl::GLuint index, gl::GLintptr offset, gl::GLsizeiptr size) const
{
    gl::BindBufferRange(target, index, m_id, offset, size);
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
