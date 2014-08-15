#include <globjects/RenderBufferAttachment.h>

#include <cassert>

#include <globjects/RenderBufferObject.h>

namespace glo
{

RenderBufferAttachment::RenderBufferAttachment(FrameBufferObject * fbo,  gl::GLenum attachment, RenderBufferObject * renderBuffer)
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

} // namespace glo
