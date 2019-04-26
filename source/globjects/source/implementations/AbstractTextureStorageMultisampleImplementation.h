
#pragma once


#include <glbinding/gl/types.h>

#include <globjects/Texture.h>


namespace globjects
{


class AbstractTextureStorageMultisampleImplementation
{
public:
    AbstractTextureStorageMultisampleImplementation();
    virtual ~AbstractTextureStorageMultisampleImplementation();

    static AbstractTextureStorageMultisampleImplementation * get(Texture::StorageMultisampleImplementation impl =
        Texture::StorageMultisampleImplementation::DirectStateAccessARB);

    virtual void storage2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSampleLocations) const = 0;
    virtual void storage3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSampleLocations) const = 0;
};


} // namespace globjects
