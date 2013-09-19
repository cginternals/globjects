#pragma once

#include <glow/glow.h>
#include <glow/ref_ptr.hpp>
#include <glow/RenderBufferObject.h>
#include <glow/FrameBufferAttachment.h>

namespace glow {

class GLOW_API RenderBufferAttachment : public FrameBufferAttachment
{
public:
	RenderBufferAttachment(RenderBufferObject* renderBuffer, GLenum attachment);

	virtual bool isRenderBufferAttachment() const;
	RenderBufferObject* renderBuffer();
protected:
	ref_ptr<RenderBufferObject> m_renderBuffer;
};

} // namespace glow
