#include <globjects/FrameBufferAttachment.h>

#include <sstream>
#include <string>

#include <globjects/constants.h>

#include <globjects/FrameBufferObject.h>

namespace glo
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
    return glo::enumName(m_attachment);
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

} // namespace glo
