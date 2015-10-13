#pragma once

#include <globjects/base/Singleton.h>

#include "AbstractTextureImplementation.h"

namespace globjects
{

class TextureImplementation_DirectStateAccessEXT : public AbstractTextureImplementation
        , public Singleton<TextureImplementation_DirectStateAccessEXT>
{
public:
    TextureImplementation_DirectStateAccessEXT();
    virtual ~TextureImplementation_DirectStateAccessEXT();

    virtual gl::GLuint create(gl::GLenum target) const override;
    virtual void destroy(gl::GLuint id) const override;
};

} // namespace globjects
