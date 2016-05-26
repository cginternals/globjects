
#pragma once

#include <glm/fwd.hpp>

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
    TextureHandle(gl::GLuint64 handle);
    explicit TextureHandle(const Texture * texture);
    TextureHandle(const Texture * texture, const Sampler * sampler);

    void makeResident();
    void makeNonResident();
    bool isResident() const;

    gl::GLuint64 handle() const;
    operator gl::GLuint64() const;

    glm::uvec2 asUVec2() const;
    operator glm::uvec2() const;

private:
    gl::GLuint64 m_handle;
};


} // namespace globjects
