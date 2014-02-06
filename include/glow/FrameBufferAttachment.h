#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>

namespace glow 
{

class TextureAttachment;
class RenderBufferAttachment;

/** \brief Wraps attachments to a FrameBufferObject.
    
    Normally, FrameBufferAttachments are created using the API of FrameBufferObject.
    
    \see FrameBufferObject
    \see TextureAttachment
    \see RenderBufferAttachment
*/
class GLOW_API FrameBufferAttachment : public Referenced
{
public:
    FrameBufferAttachment(GLenum attachment);

	GLenum attachment() const;

	virtual bool isTextureAttachment() const;
	virtual bool isRenderBufferAttachment() const;

    TextureAttachment * asTextureAttachment();
    RenderBufferAttachment * asRenderBufferAttachment();

	std::string attachmentString() const;

protected:
	GLenum m_attachment;
};

} // namespace glow
