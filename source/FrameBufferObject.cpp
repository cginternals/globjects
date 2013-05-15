#include <glow/FrameBufferObject.h>

using namespace glow;

FrameBufferObject::FrameBufferObject()
{
	glGenFramebuffers(1, &_id);
}

FrameBufferObject::~FrameBufferObject()
{
	glDeleteFramebuffers(1, &_id);
}

void FrameBufferObject::bind(GLenum target)
{
	glBindFramebuffer(target, _id);
}

void FrameBufferObject::unbind(GLenum target)
{
	glBindFramebuffer(target, 0);
}
