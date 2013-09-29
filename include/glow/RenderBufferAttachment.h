#pragma once

#include <glow/glow.h>
#include <glow/ref_ptr.h>
#include <glow/RenderBufferObject.h>
#include <glow/FrameBufferAttachment.h>

namespace glow {

/**
 * \brief The RenderBufferAttachment class encapsulates render buffer attachments of a frame buffer object.
 *
 * This class is a link between a RenderBufferObject and the FrameBufferObject it is attached to.
 * To get the attached render buffer, call renderBuffer().
 *
 * \see RenderBufferObject
 * \see FrameBufferObject
 */
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
