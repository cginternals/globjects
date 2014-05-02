#include <glow/TextureAttachment.h>

#include <cassert>

#include <glow/Texture.h>

namespace glow
{

TextureAttachment::TextureAttachment(FrameBufferObject * fbo, GLenum attachment, Texture * texture, GLint level, GLint layer)
: FrameBufferAttachment(fbo, attachment)
, m_texture(texture)
, m_level(level)
, m_layer(layer)
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

const Texture * TextureAttachment::texture() const
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
