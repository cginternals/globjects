#include <glow/FrameBufferAttachment.h>

#include <sstream>
#include <string>

#include <glow/FrameBufferObject.h>

namespace glow
{

FrameBufferAttachment::FrameBufferAttachment(FrameBufferObject * fbo, gl::GLenum attachment)
: m_fbo(fbo)
, m_attachment(attachment)
{
}

gl::GLenum FrameBufferAttachment::attachment() const
{
	return m_attachment;
}

gl::GLint FrameBufferAttachment::getParameter(gl::GLenum pname) const
{
    return m_fbo->getAttachmentParameter(m_attachment, pname);
}

bool FrameBufferAttachment::isTextureAttachment() const
{
	return false;
}

bool FrameBufferAttachment::isRenderBufferAttachment() const
{
	return false;
}

std::string FrameBufferAttachment::attachmentString() const
{
	switch (m_attachment)
	{
		case gl::DEPTH_ATTACHMENT:
			return "gl::DEPTH_ATTACHMENT";
		case gl::STENCIL_ATTACHMENT:
			return "gl::STENCIL_ATTACHMENT";
		case gl::DEPTH_STENCIL_ATTACHMENT:
			return "gl::DEPTH_STENCIL_ATTACHMENT";
	}

	if (m_attachment>=gl::COLOR_ATTACHMENT0 && gl::COLOR_ATTACHMENT0<= gl::COLOR_ATTACHMENT15)
	{
		return "gl::COLOR_ATTACHMENT"+std::to_string(m_attachment-gl::COLOR_ATTACHMENT0);
	}

	return "Unknown attachment "+std::to_string(m_attachment);
}

TextureAttachment * FrameBufferAttachment::asTextureAttachment()
{
    return isTextureAttachment() ? reinterpret_cast<TextureAttachment*>(this) : nullptr;
}

const TextureAttachment * FrameBufferAttachment::asTextureAttachment() const
{
    return isTextureAttachment() ? reinterpret_cast<const TextureAttachment*>(this) : nullptr;
}

RenderBufferAttachment * FrameBufferAttachment::asRenderBufferAttachment()
{
    return isRenderBufferAttachment() ? reinterpret_cast<RenderBufferAttachment*>(this) : nullptr;
}

const RenderBufferAttachment * FrameBufferAttachment::asRenderBufferAttachment() const
{
    return isRenderBufferAttachment() ? reinterpret_cast<const RenderBufferAttachment*>(this) : nullptr;
}

} // namespace glow
