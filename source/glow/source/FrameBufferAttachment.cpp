#include <glow/FrameBufferAttachment.h>

#include <sstream>
#include <string>

#include <glow/FrameBufferObject.h>

namespace glow
{

FrameBufferAttachment::FrameBufferAttachment(FrameBufferObject * fbo, GLenum attachment)
: m_fbo(fbo)
, m_attachment(attachment)
{
}

GLenum FrameBufferAttachment::attachment() const
{
	return m_attachment;
}

GLint FrameBufferAttachment::getParameter(GLenum pname) const
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
		case GL_DEPTH_ATTACHMENT:
			return "GL_DEPTH_ATTACHMENT";
		case GL_STENCIL_ATTACHMENT:
			return "GL_STENCIL_ATTACHMENT";
		case GL_DEPTH_STENCIL_ATTACHMENT:
			return "GL_DEPTH_STENCIL_ATTACHMENT";
	}

	if (m_attachment>=GL_COLOR_ATTACHMENT0 && GL_COLOR_ATTACHMENT0<= GL_COLOR_ATTACHMENT15)
	{
		return "GL_COLOR_ATTACHMENT"+std::to_string(m_attachment-GL_COLOR_ATTACHMENT0);
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
