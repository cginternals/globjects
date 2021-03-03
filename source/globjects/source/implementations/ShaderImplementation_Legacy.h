
#pragma once


#include <memory>

#include "../base/Singleton.h"

#include "AbstractShaderImplementation.h"


namespace globjects
{


class ShaderImplementation_Legacy : public AbstractShaderImplementation
    , public Singleton<ShaderImplementation_Legacy>
{
public:
    virtual gl::GLuint create(gl::GLenum type) const override;
    virtual void destroy(gl::GLuint id) const override;
};


} // namespace globjects
