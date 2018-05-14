
#pragma once

#include <glbinding/gl/types.h>

#include <globjects/Texture.h>


namespace globjects
{


class AbstractTextureStorageImplementation
{
public:
    AbstractTextureStorageImplementation();
    virtual ~AbstractTextureStorageImplementation();

    static AbstractTextureStorageImplementation * get(Texture::StorageImplementation impl =
        Texture::StorageImplementation::DirectStateAccessARB);

    virtual void storage1D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width) const  = 0;
    virtual void storage2D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const  = 0;
    virtual void storage3D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth) const  = 0;

    virtual void storage2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSampleLocations) const = 0;
    virtual void storage3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSampleLocations) const = 0;

    virtual void cubeMapStorage(const Texture * texture, gl::GLint levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const = 0;
};


} // namespace globjects
