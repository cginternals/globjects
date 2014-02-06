#include <glow/FrameBufferAttachment.h>

#include <sstream>
#include <string>

namespace glow
{

FrameBufferAttachment::FrameBufferAttachment(GLenum attachment)
: m_attachment(attachment)
{
}

GLenum FrameBufferAttachment::attachment() const
{
	return m_attachment;
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

RenderBufferAttachment * FrameBufferAttachment::asRenderBufferAttachment()
{
    return isRenderBufferAttachment() ? reinterpret_cast<RenderBufferAttachment*>(this) : nullptr;
}

} // namespace glow
