#include <glow/RenderBufferObject.h>

using namespace glow;

RenderBufferObject::RenderBufferObject()
{
	glGenRenderbuffers(1, &_id);
}

RenderBufferObject::~RenderBufferObject()
{
	glDeleteRenderbuffers(1, &_id);
}

void RenderBufferObject::bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, _id);
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
