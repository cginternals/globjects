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

TextureAttachment * FramebufferAttachment::asTextureAttachment()
{
    return isTextureAttachment() ? reinterpret_cast<TextureAttachment*>(this) : nullptr;
}

const TextureAttachment * FramebufferAttachment::asTextureAttachment() const
{
    return isTextureAttachment() ? reinterpret_cast<const TextureAttachment*>(this) : nullptr;
}

RenderBufferAttachment * FramebufferAttachment::asRenderBufferAttachment()
{
    return isRenderBufferAttachment() ? reinterpret_cast<RenderBufferAttachment*>(this) : nullptr;
}

const RenderBufferAttachment * FramebufferAttachment::asRenderBufferAttachment() const
{
    return isRenderBufferAttachment() ? reinterpret_cast<const RenderBufferAttachment*>(this) : nullptr;
}

} // namespace glo
