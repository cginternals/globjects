
#pragma once


#include <memory>

#include "../base/Singleton.h"

#include "AbstractRenderbufferImplementation.h"


namespace globjects
{


class RenderbufferImplementation_Legacy : public AbstractRenderbufferImplementation
    , public Singleton<RenderbufferImplementation_Legacy>
{
public:
    virtual gl::GLuint create() const override;
    virtual void destroy(gl::GLuint id) const override;
};


} // namespace globjects
