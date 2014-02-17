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

void RenderBufferObject::bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	CheckGLError();
}

void RenderBufferObject::unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	CheckGLError();
}

void RenderBufferObject::storage(GLenum internalformat, GLsizei width, GLsizei height)
{
	bind();

	glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
	CheckGLError();
}

void RenderBufferObject::storageMultisample(GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	bind();

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalformat, width, height);
	CheckGLError();
}

GLint RenderBufferObject::getParameter(GLenum pname)
{
	GLint value = 0;

	glGetRenderbufferParameteriv(GL_RENDERBUFFER, pname, &value);
	CheckGLError();

	return value;
}

} // namespace glow
