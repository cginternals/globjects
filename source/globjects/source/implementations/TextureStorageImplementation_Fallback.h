
#pragma once


#include "../base/Singleton.h"

#include "AbstractTextureStorageImplementation.h"


namespace globjects
{


class TextureStorageImplementation_Fallback : public AbstractTextureStorageImplementation
    , public Singleton<TextureStorageImplementation_Fallback>
{
public:
    TextureStorageImplementation_Fallback();
    virtual ~TextureStorageImplementation_Fallback();

    virtual void storage1D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width) const override;
    virtual void storage2D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const override;
    virtual void storage3D(const Texture * texture, gl::GLsizei levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth) const override;

    virtual void cubeMapStorage(const Texture * texture, gl::GLint levels, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height) const override;
};


} // namespace globjects
