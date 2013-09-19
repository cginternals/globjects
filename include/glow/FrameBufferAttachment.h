#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>

namespace glow {

class GLOW_API FrameBufferAttachment : public Referenced
{
public:
	FrameBufferAttachment(GLenum attachment);

	GLenum attachment() const;

	virtual bool isTextureAttachment() const;
	virtual bool isRenderBufferAttachment() const;

	std::string attachmentString() const;
protected:
	GLenum m_attachment;
};

} // namespace glow
