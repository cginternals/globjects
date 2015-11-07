#include <globjects/TextureHandle.h>

#include <glm/vec2.hpp>

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

TextureHandle::TextureHandle(const GLuint64 handle)
: m_handle(handle)
{
}

TextureHandle::TextureHandle(const Texture * texture)
: m_handle(glGetTextureHandleARB(texture->id()))
{
}

TextureHandle::TextureHandle(const Texture * texture, const Sampler * sampler)
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

bool TextureHandle::isResident() const
{
    return glIsTextureHandleResidentARB(m_handle) == GL_TRUE;
}

GLuint64 TextureHandle::handle() const
{
    return m_handle;
}

glm::uvec2 TextureHandle::asUVec2() const
{
    return glm::uvec2(static_cast<GLuint>(m_handle & 0xFFFFFFFF), m_handle >> 32);
}

TextureHandle::operator GLuint64() const
{
    return m_handle;
}

TextureHandle::operator glm::uvec2() const
{
    return asUVec2();
}

} // namespace globjects
