#pragma once

#include <globjects/base/Singleton.h>

#include "AbstractTextureImplementation.h"

namespace globjects
{

class TextureImplementation_Legacy : public AbstractTextureImplementation
    , public Singleton<TextureImplementation_Legacy>
{
public:
    TextureImplementation_Legacy();
    virtual ~TextureImplementation_Legacy();

    virtual gl::GLuint create(gl::GLenum target) const override;
    virtual void destroy(gl::GLuint id) const override;
};

} // namespace globjects
