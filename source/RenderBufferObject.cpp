#include <glow/RenderBufferObject.h>

using namespace glow;

RenderBufferObject::RenderBufferObject()
: Object(genRenderBuffer())
{
}

void RenderBufferObject::deleteGLObject()
{
	glDeleteRenderbuffers(1, &m_id);
}

GLuint RenderBufferObject::genRenderBuffer()
{
	GLuint id = 0;
	glGenRenderbuffers(1, &id);
	return id;
}

void RenderBufferObject::bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

void RenderBufferObject::unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBufferObject::storage(GLenum internalformat, GLsizei width, GLsizei height)
{
	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
	unbind();
}

void RenderBufferObject::storageMultisample(GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	bind();
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalformat, width, height);
	unbind();
}
