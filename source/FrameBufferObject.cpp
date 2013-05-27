#include <glow/FrameBufferObject.h>

using namespace glow;

FrameBufferObject::FrameBufferObject()
: Object(genFrameBuffer())
, _target(GL_FRAMEBUFFER)
{
}

FrameBufferObject::~FrameBufferObject()
{
	if (_id) glDeleteFramebuffers(1, &_id);
}

GLuint FrameBufferObject::genFrameBuffer()
{
	GLuint id = 0;
	glGenFramebuffers(1, &id);
	return id;
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
