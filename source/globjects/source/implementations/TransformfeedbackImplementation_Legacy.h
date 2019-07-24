
#pragma once


#include <memory>

#include "../base/Singleton.h"

#include "AbstractTransformfeedbackImplementation.h"


namespace globjects
{


class TransformfeedbackImplementation_Legacy : public AbstractTransformfeedbackImplementation
    , public Singleton<TransformfeedbackImplementation_Legacy>
{
public:
    virtual gl::GLuint create() const override;
    virtual void destroy(gl::GLuint id) const override;
};


} // namespace globjects
