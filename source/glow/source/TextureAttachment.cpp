#include <glow/TextureAttachment.h>

#include <cassert>

#include <glow/Texture.h>

namespace glow
{

TextureAttachment::TextureAttachment(Texture * texture, GLenum attachment, GLint level, GLint layer)
: FrameBufferAttachment(attachment)
, m_texture(texture)
, m_level(level)
, m_layer(layer)
{
    assert(texture != nullptr);
}

bool TextureAttachment::isTextureAttachment() const
{
	return true;
}

Texture * TextureAttachment::texture()
{
	return m_texture;
}

GLint TextureAttachment::level() const
{
    return m_level;
}

bool TextureAttachment::hasLayer() const
{
    return m_layer >= 0;
}

GLint TextureAttachment::layer() const
{
    return m_layer;
}

} // namespace glow
