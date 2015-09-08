#pragma once

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

namespace globjects 
{

class Texture;
class Sampler;

class GLOBJECTS_API TextureHandle
{
public:
    TextureHandle();
    explicit TextureHandle(const Texture* texture);
    TextureHandle(const Texture* texture, const Sampler* sampler);

    void makeResident();
    void makeNonResident();
    bool isResident();

    gl::GLuint64 handle() const;
    operator gl::GLuint64() const;

private:
    gl::GLuint64 m_handle;
};

} // namespace globjects
