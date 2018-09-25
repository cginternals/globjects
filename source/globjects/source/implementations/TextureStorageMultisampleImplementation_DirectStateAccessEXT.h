
#pragma once


#include "../base/Singleton.h"

#include "AbstractTextureStorageMultisampleImplementation.h"


namespace globjects
{


class TextureStorageMultisampleImplementation_DirectStateAccessEXT : public AbstractTextureStorageMultisampleImplementation
        , public Singleton<TextureStorageMultisampleImplementation_DirectStateAccessEXT>
{
public:
    TextureStorageMultisampleImplementation_DirectStateAccessEXT();
    virtual ~TextureStorageMultisampleImplementation_DirectStateAccessEXT();

    virtual void storage2DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLboolean fixedSampleLocations) const override;
    virtual void storage3DMultisample(const Texture * texture, gl::GLsizei samples, gl::GLenum internalFormat, gl::GLsizei width, gl::GLsizei height, gl::GLsizei depth, gl::GLboolean fixedSampleLocations) const override;
};


} // namespace globjects
