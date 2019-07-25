
#include "AbstractTransformFeedbackImplementation.h"

#include <globjects/globjects.h>
#include <glbinding/gl/extension.h>

#include "TransformFeedbackImplementation_Legacy.h"


using namespace gl;


namespace globjects 
{


AbstractTransformFeedbackImplementation::AbstractTransformFeedbackImplementation()
{
}

AbstractTransformFeedbackImplementation::~AbstractTransformFeedbackImplementation()
{
}

AbstractTransformFeedbackImplementation * AbstractTransformFeedbackImplementation::get(const TransformFeedback::Implementation /*impl*/)
{
    return TransformFeedbackImplementation_Legacy::instance();
}


} // namespace globjects
