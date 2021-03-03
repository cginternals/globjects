
#pragma once


#include <memory>

#include "../base/Singleton.h"

#include "AbstractSamplerImplementation.h"


namespace globjects
{


class SamplerImplementation_Legacy : public AbstractSamplerImplementation
    , public Singleton<SamplerImplementation_Legacy>
{
public:
    virtual gl::GLuint create() const override;
    virtual void destroy(gl::GLuint id) const override;
};


} // namespace globjects
