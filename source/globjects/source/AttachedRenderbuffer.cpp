#include <globjects/AttachedRenderbuffer.h>

#include <cassert>

#include <globjects/RenderBufferObject.h>

namespace glo
{

AttachedRenderbuffer::AttachedRenderbuffer(Framebuffer * fbo,  gl::GLenum attachment, RenderBufferObject * renderBuffer)
: FramebufferAttachment(fbo, attachment)
, m_renderBuffer(renderBuffer)
{
}

bool AttachedRenderbuffer::isRenderBufferAttachment() const
{
	return true;
}

RenderBufferObject * AttachedRenderbuffer::renderBuffer()
{
	return m_renderBuffer;
}

const RenderBufferObject * AttachedRenderbuffer::renderBuffer() const
{
    return m_renderBuffer;
}

} // namespace glo
