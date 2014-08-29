#include <globjects/FramebufferAttachment.h>

#include <sstream>
#include <string>

#include <globjects/constants.h>

#include <globjects/Framebuffer.h>

namespace glo
{

FramebufferAttachment::FramebufferAttachment(Framebuffer * fbo, gl::GLenum attachment)
: m_fbo(fbo)
, m_attachment(attachment)
{
}

gl::GLenum FramebufferAttachment::attachment() const
{
	return m_attachment;
}

gl::GLint FramebufferAttachment::getParameter(gl::GLenum pname) const
{
    return m_fbo->getAttachmentParameter(m_attachment, pname);
}

bool FramebufferAttachment::isTextureAttachment() const
{
	return false;
}

bool FramebufferAttachment::isRenderBufferAttachment() const
{
	return false;
}

std::string FramebufferAttachment::attachmentString() const
{
    return glo::enumName(m_attachment);
}

AttachedTexture * FramebufferAttachment::asTextureAttachment()
{
    return isTextureAttachment() ? reinterpret_cast<AttachedTexture*>(this) : nullptr;
}

const AttachedTexture * FramebufferAttachment::asTextureAttachment() const
{
    return isTextureAttachment() ? reinterpret_cast<const AttachedTexture*>(this) : nullptr;
}

AttachedRenderbuffer * FramebufferAttachment::asRenderBufferAttachment()
{
    return isRenderBufferAttachment() ? reinterpret_cast<AttachedRenderbuffer*>(this) : nullptr;
}

const AttachedRenderbuffer * FramebufferAttachment::asRenderBufferAttachment() const
{
    return isRenderBufferAttachment() ? reinterpret_cast<const AttachedRenderbuffer*>(this) : nullptr;
}

} // namespace glo
