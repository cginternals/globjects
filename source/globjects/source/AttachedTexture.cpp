#include <globjects/AttachedTexture.h>

#include <cassert>

#include <globjects/Texture.h>

namespace glo
{

AttachedTexture::AttachedTexture(Framebuffer * fbo, gl::GLenum attachment, Texture * texture, gl::GLint level, gl::GLint layer)
: FramebufferAttachment(fbo, attachment)
, m_texture(texture)
, m_level(level)
, m_layer(layer)
{
}

bool AttachedTexture::isTextureAttachment() const
{
	return true;
}

Texture * AttachedTexture::texture()
{
	return m_texture;
}

const Texture * AttachedTexture::texture() const
{
    return m_texture;
}

gl::GLint AttachedTexture::level() const
{
    return m_level;
}

bool AttachedTexture::hasLayer() const
{
    return m_layer >= 0;
}

gl::GLint AttachedTexture::layer() const
{
    return m_layer;
}

} // namespace glo
