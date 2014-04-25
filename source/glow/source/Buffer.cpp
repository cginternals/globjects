#include <glow/Buffer.h>

#include <cassert>

#include <glow/Error.h>
#include <glow/global.h>
#include <glow/ObjectVisitor.h>

#include <glow/Registry.h>
#include <glow/BehaviorRegistry.h>

#include "behaviors/AbstractBufferBehavior.h"

namespace glow
{

Buffer::Buffer()
: Object(genBuffer())
, m_target(0)
{
}

Buffer::Buffer(GLenum target)
: Object(genBuffer())
, m_target(target)
{
}

Buffer::Buffer(GLuint id, GLenum target)
: Object(id, false)
, m_target(target)
{
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

const AbstractBufferBehavior & Buffer::behavior() const
{
    return Registry::current().behaviors().bufferBehavior();
}

void Buffer::accept(ObjectVisitor& visitor)
{
	visitor.visitBuffer(this);
}

void Buffer::bind() const
{
    glBindBuffer(m_target, m_id);
	CheckGLError();
}

void Buffer::bind(GLenum target) const
{
    m_target = target;
	bind();
}

void Buffer::unbind() const
{
    glBindBuffer(m_target, 0);
	CheckGLError();
}

void Buffer::unbind(GLenum target)
{
    glBindBuffer(target, 0);
    CheckGLError();
}

GLenum Buffer::target() const
{
    return m_target;
}

const void * Buffer::map() const
{
    return static_cast<const void*>(behavior().map(this, GL_READ_ONLY));
}

void* Buffer::map(GLenum access)
{
    return behavior().map(this, access);
}

void* Buffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access)
{
    return behavior().mapRange(this, offset, length, access);
}

bool Buffer::unmap() const
{
    return behavior().unmap(this);
}

void Buffer::setData(GLsizeiptr size, const GLvoid * data, GLenum usage)
{
    behavior().setData(this, size, data, usage);
}
    
void Buffer::setSubData(GLintptr offset, GLsizeiptr size, const GLvoid * data)
{
    behavior().setSubData(this, offset, size, data);
}

void Buffer::setStorage(GLsizeiptr size, const GLvoid * data, GLbitfield flags)
{
    behavior().setStorage(this, size, data, flags);
}

GLint Buffer::getParameter(GLenum pname) const
{
    GLint value = 0;

    behavior().getParameter(this, pname, &value);

    return value;
}

void Buffer::bindBase(GLuint index) const
{
    glBindBufferBase(m_target, index, m_id);
    CheckGLError();
}

void Buffer::bindBase(GLenum target, GLuint index) const
{
    m_target = target;
    bindBase(index);
}

void Buffer::bindRange(GLuint index, GLintptr offset, GLsizeiptr size) const
{
    glBindBufferRange(m_target, index, m_id, offset, size);
    CheckGLError();
}

void Buffer::bindRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size) const
{
    m_target = target;
    bindRange(index, offset, size);
}

void Buffer::unbindIndex(GLenum target, GLuint index)
{
    glBindBufferBase(target, index, 0);
    CheckGLError();
}

void Buffer::copySubData(glow::Buffer * buffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const
{
    assert(buffer != nullptr);

    behavior().copySubData(this, buffer, readOffset, writeOffset, size);
}

void Buffer::copySubData(glow::Buffer * buffer, GLsizeiptr size) const
{
	copySubData(buffer, 0, 0, size);
}

void Buffer::copyData(glow::Buffer * buffer, GLsizeiptr size, GLenum usage) const
{
    assert(buffer != nullptr);

	buffer->setData(static_cast<GLsizei>(size), nullptr, usage);
	copySubData(buffer, 0, 0, size);
}

void Buffer::clearData(GLenum internalformat, GLenum format, GLenum type, const void * data)
{
    behavior().clearData(this, internalformat, format, type, data);
}

void Buffer::clearSubData(GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data)
{
    behavior().clearSubData(this, internalformat, offset, size, format, type, data);
}

} // namespace glow
