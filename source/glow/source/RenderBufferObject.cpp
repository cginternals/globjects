#include <glow/RenderBufferObject.h>

#include <cmath>

#include <glow/Error.h>
#include <glow/ObjectVisitor.h>

namespace glow
{

RenderBufferObject::RenderBufferObject()
: Object(genRenderBuffer())
{
}

RenderBufferObject::~RenderBufferObject()
{
	if (ownsGLObject())
	{
		glDeleteRenderbuffers(1, &m_id);
		CheckGLError();
	}
}

GLuint RenderBufferObject::genRenderBuffer()
{
	GLuint id = 0;

	glGenRenderbuffers(1, &id);
	CheckGLError();

	return id;
}

void RenderBufferObject::accept(ObjectVisitor& visitor)
{
	visitor.visitRenderBufferObject(this);
}

void RenderBufferObject::bind(GLenum target) const
{
    glBindRenderbuffer(target, m_id);
	CheckGLError();
}

void RenderBufferObject::unbind(GLenum target)
{
    glBindRenderbuffer(target, 0);
	CheckGLError();
}

void RenderBufferObject::storage(GLenum internalformat, GLsizei width, GLsizei height)
{
    bind(gl::RENDERBUFFER);

	glRenderbufferStorage(gl::RENDERBUFFER, internalformat, width, height);
	CheckGLError();
}

void RenderBufferObject::storageMultisample(GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
    bind(gl::RENDERBUFFER);

	glRenderbufferStorageMultisample(gl::RENDERBUFFER, samples, internalformat, width, height);
	CheckGLError();
}

GLint RenderBufferObject::getParameter(GLenum pname) const
{
	GLint value = 0;

    bind(gl::RENDERBUFFER);

	glGetRenderbufferParameteriv(gl::RENDERBUFFER, pname, &value);
	CheckGLError();

	return value;
}

} // namespace glow
