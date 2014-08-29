#include <globjects/RenderBufferAttachment.h>

#include <cassert>

#include <globjects/RenderBufferObject.h>

namespace glo
{

RenderBufferAttachment::RenderBufferAttachment(Framebuffer * fbo,  gl::GLenum attachment, RenderBufferObject * renderBuffer)
: FramebufferAttachment(fbo, attachment)
, m_renderBuffer(renderBuffer)
{
}

bool RenderBufferAttachment::isRenderBufferAttachment() const
{
	return true;
}

RenderBufferObject * RenderBufferAttachment::renderBuffer()
{
	return m_renderBuffer;
}

const RenderBufferObject * RenderBufferAttachment::renderBuffer() const
{
    return m_renderBuffer;
}

} // namespace glo
