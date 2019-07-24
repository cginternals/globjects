
#include "AbstractTransformfeedbackImplementation.h"

#include <globjects/globjects.h>
#include <glbinding/gl/extension.h>

#include "TransformfeedbackImplementation_Legacy.h"


using namespace gl;


namespace globjects 
{


AbstractTransformfeedbackImplementation::AbstractTransformfeedbackImplementation()
{
}

AbstractTransformfeedbackImplementation::~AbstractTransformfeedbackImplementation()
{
}

AbstractTransformfeedbackImplementation * AbstractTransformfeedbackImplementation::get(const TransformFeedback::Implementation /*impl*/)
{
    return TransformfeedbackImplementation_Legacy::instance();
}


} // namespace globjects
