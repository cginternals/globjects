#pragma once

#include <glbinding/gl/types.h>

#include <globjects/Texture.h>

namespace globjects
{

class AbstractTextureImplementation
{
public:
    AbstractTextureImplementation();
    virtual ~AbstractTextureImplementation();

    static AbstractTextureImplementation * get(Texture::BindlessImplementation impl =
        Texture::BindlessImplementation::DirectStateAccessARB);

    virtual gl::GLuint create(gl::GLenum target) const = 0;
    virtual void destroy(gl::GLuint id) const = 0;
};

} // namespace globjects
