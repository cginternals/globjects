#include <glow/Buffer.h>

#include <cassert>

#include <glow/Error.h>
#include <glow/Extension.h>
#include <glow/ObjectVisitor.h>

namespace glow
{

Buffer::Buffer()
: Object(genBuffer())
, m_target(0)
, m_directStateAccess(hasExtension(GLOW_EXT_direct_state_access))
{
}

Buffer::Buffer(GLenum target)
: Object(genBuffer())
, m_target(target)
, m_directStateAccess(hasExtension(GLOW_EXT_direct_state_access))
{
}

Buffer::Buffer(GLuint id, GLenum target)
: Object(id, false)
, m_target(target)
, m_directStateAccess(hasExtension(GLOW_EXT_direct_state_access))
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

void Buffer::accept(ObjectVisitor& visitor)
{
	visitor.visitBuffer(this);
}

void Buffer::bind()
{
    glBindBuffer(m_target, m_id);
	CheckGLError();
}

void Buffer::bind(GLenum target)
{
    m_target = target;
	bind();
}

void Buffer::unbind()
{
    glBindBuffer(m_target, 0);
	CheckGLError();
}

void Buffer::unbind(GLenum target)
{
    glBindBuffer(target, 0);
    CheckGLError();
}

void* Buffer::map(GLenum access)
{
    if (m_directStateAccess)
    {
        void* result = glMapNamedBufferEXT(m_id, access);
        CheckGLError();
        return result;
    }
    else
    {
        bind();

        void* result = glMapBuffer(m_target, access);
        CheckGLError();
        return result;
    }

}

void* Buffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access)
{
    if (m_directStateAccess)
    {
        void* result = glMapNamedBufferRangeEXT(m_id, offset, length, access);
        CheckGLError();
        return result;
    }
    else
    {
        bind();

        void* result = glMapBufferRange(m_target, offset, length, access);
        CheckGLError();
        return result;
    }
}

bool Buffer::unmap()
{
    if (m_directStateAccess)
    {
        GLboolean success = glUnmapNamedBufferEXT(m_id);
        CheckGLError();

        return success == GL_TRUE;
    }
    else
    {
        bind();

        GLboolean success = glUnmapBuffer(m_target);
        CheckGLError();

        return success == GL_TRUE;
    }
}

void Buffer::setData(GLsizeiptr size, const GLvoid* data, GLenum usage)
{
    if (m_directStateAccess)
    {
        glNamedBufferDataEXT(m_id, size, data, usage);
    }
    else
    {
        bind();
        glBufferData(m_target, size, data, usage);
        CheckGLError();
    }
}
    
void Buffer::setSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
    if (m_directStateAccess)
    {
        glNamedBufferSubDataEXT(m_id, offset, size, data);
        CheckGLError();
    }
    else
    {
        bind();
        glBufferSubData(m_target, offset, size, data);
        CheckGLError();
    }
}

void Buffer::setStorage(GLsizeiptr size, const GLvoid * data, GLbitfield flags)
{
    if (m_directStateAccess)
    {
        glNamedBufferStorageEXT(m_id, size, data, flags);
        CheckGLError();
    }
    else
    {
        bind();
        glBufferStorage(m_target, size, data, flags);
        CheckGLError();
    }
}

GLint Buffer::getParameter(GLenum pname)
{
    if (m_directStateAccess)
    {
        GLint value = 0;

        glGetNamedBufferParameterivEXT(m_id, pname, &value);
        CheckGLError();

        return value;
    }
    else
    {
        bind();

        GLint value = 0;

        glGetBufferParameteriv(m_target, pname, &value);
        CheckGLError();

        return value;
    }
}

void Buffer::bindBase(GLenum target, GLuint index)
{
    glBindBufferBase(target, index, m_id);
    CheckGLError();
}

void Buffer::bindRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size)
{
    glBindBufferRange(target, index, m_id, offset, size);
	CheckGLError();
}

void Buffer::unbindIndex(GLenum target, GLuint index)
{
    glBindBufferBase(target, index, 0);
    CheckGLError();
}

void Buffer::copySubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
    glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
    CheckGLError();
}

void Buffer::copySubData(GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
    glBindBuffer(GL_COPY_READ_BUFFER, m_id);
    CheckGLError();

	copySubData(GL_COPY_READ_BUFFER, writeTarget, readOffset, writeOffset, size);

    glBindBuffer(GL_COPY_READ_BUFFER, 0);
    CheckGLError();
}

void Buffer::copySubData(GLenum writeTarget, GLsizeiptr size)
{
    glBindBuffer(GL_COPY_READ_BUFFER, m_id);
    CheckGLError();

	copySubData(GL_COPY_READ_BUFFER, writeTarget, 0, 0, size);

    glBindBuffer(GL_COPY_READ_BUFFER, 0);
    CheckGLError();
}

void Buffer::copySubData(glow::Buffer* buffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
    assert(buffer != nullptr);

    if (m_directStateAccess)
    {
        glNamedCopyBufferSubDataEXT(m_id, buffer->id(), readOffset, writeOffset, size);
        CheckGLError();
    }
    else
    {
        glBindBuffer(GL_COPY_WRITE_BUFFER, buffer->id());
        CheckGLError();

        copySubData(GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size);

        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
        CheckGLError();
    }
}

void Buffer::copySubData(glow::Buffer* buffer, GLsizeiptr size)
{
    assert(buffer != nullptr);

	copySubData(buffer, 0, 0, size);
}

void Buffer::copyData(glow::Buffer* buffer, GLsizeiptr size, GLenum usage)
{
    assert(buffer != nullptr);

	buffer->setData(static_cast<GLsizei>(size), nullptr, usage);
	copySubData(buffer, 0, 0, size);
}

void Buffer::clearData(GLenum internalformat, GLenum format, GLenum type, const void* data)
{
    if (m_directStateAccess)
    {
        glClearNamedBufferDataEXT(m_id, internalformat, format, type, data);
        CheckGLError();
    }
    else
    {
        bind();

        glClearBufferData(m_target, internalformat, format, type, data);
        CheckGLError();
    }
}

void Buffer::clearSubData(GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void* data)
{
    if (m_directStateAccess)
    {
        glClearNamedBufferSubDataEXT(m_id, internalformat, offset, size, format, type, data);
        CheckGLError();
    }
    else
    {
        bind();

        glClearBufferSubData(m_target, internalformat, offset, size, format, type, data);
        CheckGLError();
    }
}

} // namespace glow
