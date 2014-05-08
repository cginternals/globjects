#pragma once

#include <string>

#include <glbinding/types.h>

#include <glowbase/Referenced.h>

#include <glow/glow_api.h>

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
    FrameBufferAttachment(FrameBufferObject * fbo, gl::GLenum attachment);

	gl::GLenum attachment() const;

    gl::GLint getParameter(gl::GLenum pname) const;

	virtual bool isTextureAttachment() const;
	virtual bool isRenderBufferAttachment() const;

    TextureAttachment * asTextureAttachment();
    const TextureAttachment * asTextureAttachment() const;
    RenderBufferAttachment * asRenderBufferAttachment();
    const RenderBufferAttachment * asRenderBufferAttachment() const;

	std::string attachmentString() const;

protected:
    FrameBufferObject * m_fbo;
	gl::GLenum m_attachment;
};

} // namespace glow
