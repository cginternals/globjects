
#pragma once


#include <memory>

#include "../base/Singleton.h"

#include "AbstractTransformFeedbackImplementation.h"


namespace globjects
{


class TransformFeedbackImplementation_Legacy : public AbstractTransformFeedbackImplementation
    , public Singleton<TransformFeedbackImplementation_Legacy>
{
public:
    virtual gl::GLuint create() const override;
    virtual void destroy(gl::GLuint id) const override;
};


} // namespace globjects
