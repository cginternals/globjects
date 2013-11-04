
#include <glow/Texture.h>
#include <glow/TextureAttachment.h>

namespace glow
{

TextureAttachment::TextureAttachment(Texture * texture, GLenum attachment)
: FrameBufferAttachment(attachment)
, m_texture(texture)
{
}

bool TextureAttachment::isTextureAttachment() const
{
	return true;
}

Texture * TextureAttachment::texture()
{
	return m_texture;
}

} // namespace glow
