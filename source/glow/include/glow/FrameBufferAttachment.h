#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow_api.h>
#include <glow/Referenced.h>

namespace glow 
{

class TextureAttachment;
class RenderBufferAttachment;
class FrameBufferObject;

/** \brief Wraps attachments to a FrameBufferObject.
    
    Normally, FrameBufferAttachments are created using the API of FrameBufferObject.
    
    \see FrameBufferObject
    \see TextureAttachment
    \see RenderBufferAttachment
*/
class GLOW_API FrameBufferAttachment : public Referenced
{
public:
    FrameBufferAttachment(FrameBufferObject * fbo, GLenum attachment);

	GLenum attachment() const;

    GLint getParameter(GLenum pname) const;

	virtual bool isTextureAttachment() const;
	virtual bool isRenderBufferAttachment() const;

    TextureAttachment * asTextureAttachment();
    const TextureAttachment * asTextureAttachment() const;
    RenderBufferAttachment * asRenderBufferAttachment();
    const RenderBufferAttachment * asRenderBufferAttachment() const;

	std::string attachmentString() const;

protected:
    FrameBufferObject * m_fbo;
	GLenum m_attachment;
};

} // namespace glow
