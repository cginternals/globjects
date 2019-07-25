
#pragma once


#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/TransformFeedback.h>


namespace globjects
{


class AbstractTransformFeedbackImplementation
{
public:
    AbstractTransformFeedbackImplementation();
    virtual ~AbstractTransformFeedbackImplementation();

    static AbstractTransformFeedbackImplementation * get(TransformFeedback::Implementation impl =
        TransformFeedback::Implementation::Legacy);

    virtual gl::GLuint create() const = 0;
    virtual void destroy(gl::GLuint id) const = 0;
};


} // namespace globjects
