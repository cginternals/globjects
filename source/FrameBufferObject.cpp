#include <glow/FrameBufferObject.h>

using namespace glow;

FrameBufferObject::FrameBufferObject()
: _target(GL_FRAMEBUFFER)
{
	glGenFramebuffers(1, &_id);
}

FrameBufferObject::~FrameBufferObject()
{
	glDeleteFramebuffers(1, &_id);
}

void FrameBufferObject::bind(GLenum target)
{
	_target = target;
	glBindFramebuffer(target, _id);
}

void FrameBufferObject::unbind()
{
	glBindFramebuffer(_target, 0);
}
