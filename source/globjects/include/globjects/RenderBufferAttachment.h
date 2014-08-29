#pragma once

#include <globjects-base/ref_ptr.h>

#include <globjects/globjects_api.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/RenderBufferObject.h>

namespace glo 
{

class Framebuffer;

/** \brief Wrapper of render buffer attachments of a frame buffer object.
    
    This class is a link between a RenderBufferObject and the FrameBufferObject
    it is attached to. To get the attached render buffer, call renderBuffer().
    
    \see RenderBufferObject
    \see FrameBufferObject
 */
class GLOBJECTS_API RenderBufferAttachment : public FramebufferAttachment
{
public:
    RenderBufferAttachment(Framebuffer * fbo,  gl::GLenum attachment, RenderBufferObject * renderBuffer);

    virtual bool isRenderBufferAttachment() const override;
	RenderBufferObject * renderBuffer();
    const RenderBufferObject * renderBuffer() const;
protected:
    ref_ptr<RenderBufferObject> m_renderBuffer;
};

} // namespace glo
