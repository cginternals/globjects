#include <glow/Buffer.h>

#include <cassert>

#include <glow/Error.h>
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

void Buffer::setWorkingTarget(GLenum target)
{
    LegacyBufferImplementation::s_workingTarget = target;
}

Buffer::Buffer()
: Object(genBuffer())
{
}

Buffer::Buffer(GLuint id, bool takeOwnership)
: Object(id, takeOwnership)
{
}

Buffer * Buffer::fromId(GLuint id, bool takeOwnership)
{
    return new Buffer(id, takeOwnership);
}

GLuint Buffer::genBuffer()
{
	GLuint id = 0;

	glGenBuffers(1, &id);
	CheckGLError();

	return id;
}

Buffer::~Buffer()
{
	if (ownsGLObject())
	{
		glDeleteBuffers(1, &m_id);
		CheckGLError();
	}
}

void Buffer::accept(ObjectVisitor& visitor)
{
	visitor.visitBuffer(this);
}

void Buffer::bind(GLenum target) const
{
    glBindBuffer(target, m_id);
    CheckGLError();
}

void Buffer::unbind(GLenum target)
{
    glBindBuffer(target, 0);
    CheckGLError();
}

void Buffer::unbind(GLenum target, GLuint index)
{
    glBindBufferBase(target, index, 0);
    CheckGLError();
}

const void * Buffer::map() const
{
    return static_cast<const void*>(implementation().map(this, GL_READ_ONLY));
}

void* Buffer::map(GLenum access)
{
    return implementation().map(this, access);
}

void* Buffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access)
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

void Buffer::setStorage(GLsizeiptr size, const GLvoid * data, GLbitfield flags)
{
    implementation().setStorage(this, size, data, flags);
}

GLint Buffer::getParameter(GLenum pname) const
{
    GLint value = 0;

    implementation().getParameter(this, pname, &value);

    return value;
}

void Buffer::bindBase(GLenum target, GLuint index) const
{
    glBindBufferBase(target, index, m_id);
    CheckGLError();
}

void Buffer::bindRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size) const
{
    glBindBufferRange(target, index, m_id, offset, size);
    CheckGLError();
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

} // namespace glow
