#pragma once

#include <glow/glow.h>
#include <glow/ref_ptr.h>
#include <glow/FrameBufferAttachment.h>
#include <glow/Texture.h>

namespace glow {

class GLOW_API TextureAttachment : public FrameBufferAttachment
{
public:
	TextureAttachment(Texture * texture, GLenum attachment);

	virtual bool isTextureAttachment() const;
	Texture* texture();
protected:
	ref_ptr<Texture> m_texture;
};

} // namespace glow
