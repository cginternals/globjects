#pragma once

#include <globjects-base/ref_ptr.h>

#include <globjects/globjects_api.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/Renderbuffer.h>

namespace glo 
{

class Framebuffer;

/** \brief Wrapper of render buffer attachments of a frame buffer object.
    
    This class is a link between a RenderBufferObject and the FrameBufferObject
    it is attached to. To get the attached render buffer, call renderBuffer().
    
    \see RenderBufferObject
    \see FrameBufferObject
 */
class GLOBJECTS_API AttachedRenderbuffer : public FramebufferAttachment
{
public:
    AttachedRenderbuffer(Framebuffer * fbo,  gl::GLenum attachment, Renderbuffer * renderBuffer);

    virtual bool isRenderBufferAttachment() const override;
	Renderbuffer * renderBuffer();
    const Renderbuffer * renderBuffer() const;
protected:
    ref_ptr<Renderbuffer> m_renderBuffer;
};

} // namespace glo
