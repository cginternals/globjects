#include <globjects/TextureHandle.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Texture.h>
#include <globjects/Sampler.h>

using namespace gl;

namespace globjects
{

TextureHandle::TextureHandle()
: m_handle(0)
{
}

TextureHandle::TextureHandle(const Texture* texture)
: m_handle(glGetTextureHandleARB(texture->id()))
{
}

TextureHandle::TextureHandle(const Texture* texture, const Sampler* sampler)
: m_handle(glGetTextureSamplerHandleARB(texture->id(), sampler->id()))
{
}

void TextureHandle::makeResident()
{
    glMakeTextureHandleResidentARB(m_handle);
}

void TextureHandle::makeNonResident()
{
    glMakeTextureHandleNonResidentARB(m_handle);
}

bool TextureHandle::isResident()
{
    return glIsTextureHandleResidentARB(m_handle) == GL_TRUE;
}

GLuint64 TextureHandle::handle() const
{
    return m_handle;
}

TextureHandle::operator GLuint64() const
{
    return m_handle;
}

} // namespace globjects
