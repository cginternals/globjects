
#pragma once


#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/TransformFeedback.h>


namespace globjects
{


class AbstractTransformfeedbackImplementation
{
public:
    AbstractTransformfeedbackImplementation();
    virtual ~AbstractTransformfeedbackImplementation();

    static AbstractTransformfeedbackImplementation * get(TransformFeedback::Implementation impl =
        TransformFeedback::Implementation::Legacy);

    virtual gl::GLuint create() const = 0;
    virtual void destroy(gl::GLuint id) const = 0;
};


} // namespace globjects
