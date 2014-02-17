#include <glow/RenderBufferAttachment.h>

#include <cassert>

#include <glow/RenderBufferObject.h>

namespace glow
{

RenderBufferAttachment::RenderBufferAttachment(
    RenderBufferObject * renderBuffer
,   GLenum attachment)
: FrameBufferAttachment(attachment)
, m_renderBuffer(renderBuffer)
{
    assert(renderBuffer != nullptr);
}

bool RenderBufferAttachment::isRenderBufferAttachment() const
{
	return true;
}

RenderBufferObject * RenderBufferAttachment::renderBuffer()
{
	return m_renderBuffer;
}

} // namespace glow
