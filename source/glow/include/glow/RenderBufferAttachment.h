#pragma once

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/ref_ptr.h>
#include <glow/FrameBufferAttachment.h>

namespace glow 
{

class RenderBufferObject;


/** \brief Wrapper of render buffer attachments of a frame buffer object.
    
    This class is a link between a RenderBufferObject and the FrameBufferObject
    it is attached to. To get the attached render buffer, call renderBuffer().
    
    \see RenderBufferObject
    \see FrameBufferObject
 */
class GLOW_API RenderBufferAttachment : public FrameBufferAttachment
{
public:
	RenderBufferAttachment(
        RenderBufferObject * renderBuffer
    ,   GLenum attachment);

    virtual bool isRenderBufferAttachment() const override;
	RenderBufferObject * renderBuffer();
    const RenderBufferObject * renderBuffer() const;
protected:
	ref_ptr<RenderBufferObject> m_renderBuffer;
};

} // namespace glow
