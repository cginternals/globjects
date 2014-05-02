#include <glow/RenderBufferAttachment.h>

#include <cassert>

#include <glow/RenderBufferObject.h>

namespace glow
{

RenderBufferAttachment::RenderBufferAttachment(FrameBufferObject * fbo,  GLenum attachment, RenderBufferObject * renderBuffer)
: FrameBufferAttachment(fbo, attachment)
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

} // namespace glow
