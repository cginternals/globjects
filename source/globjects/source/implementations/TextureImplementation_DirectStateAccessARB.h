#pragma once

#include <globjects/base/Singleton.h>

#include "AbstractTextureImplementation.h"

namespace globjects
{

class TextureImplementation_DirectStateAccessARB : public AbstractTextureImplementation
    , public Singleton<TextureImplementation_DirectStateAccessARB>
{
public:
    TextureImplementation_DirectStateAccessARB();
    virtual ~TextureImplementation_DirectStateAccessARB();

    virtual gl::GLuint create(gl::GLenum target) const override;
    virtual void destroy(gl::GLuint id) const override;
};

} // namespace globjects
