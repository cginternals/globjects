
#pragma once

#include "../base/Singleton.h"

#include "AbstractTextureStorageImplementation.h"


namespace globjects
{


class TextureStorageImplementation_DirectStateAccessEXT : public AbstractTextureStorageImplementation
        , public Singleton<TextureStorageImplementation_DirectStateAccessEXT>
{
public:
    TextureStorageImplementation_DirectStateAccessEXT();
    virtual ~TextureStorageImplementation_DirectStateAccessEXT();

    virtual void storage1D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width) const override;
    virtual void storage2D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const override;
    virtual void storage3D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth) const override;

    virtual void storage2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSampleLocations) const override;
    virtual void storage3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSampleLocations) const override;

    virtual void cubeMapStorage(const Texture * texture, gl::GLint levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const override;
};


} // namespace globjects
