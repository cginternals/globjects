
#pragma once

#include <string>

#include <glbinding/gl/types.h>

#include <globjects/base/Referenced.h>

#include <globjects/globjects_api.h>


namespace globjects 
{


class AttachedTexture;
class AttachedRenderbuffer;
class Framebuffer;

/** \brief Wraps attachments to a FrameBufferObject.
    
    Normally, FrameBufferAttachments are created using the API of FrameBufferObject.
    
    \see FrameBufferObject
    \see TextureAttachment
    \see RenderBufferAttachment
*/
class GLOBJECTS_API FramebufferAttachment : public Referenced
{
public:
    FramebufferAttachment(Framebuffer * fbo, gl::GLenum attachment);

	gl::GLenum attachment() const;

    gl::GLint getParameter(gl::GLenum pname) const;

	virtual bool isTextureAttachment() const;
	virtual bool isRenderBufferAttachment() const;

    AttachedTexture * asTextureAttachment();
    const AttachedTexture * asTextureAttachment() const;
    AttachedRenderbuffer * asRenderBufferAttachment();
    const AttachedRenderbuffer * asRenderBufferAttachment() const;

	std::string attachmentString() const;

protected:
    Framebuffer * m_fbo; // TODO: weak pointer?
	gl::GLenum m_attachment;
};


} // namespace globjects
