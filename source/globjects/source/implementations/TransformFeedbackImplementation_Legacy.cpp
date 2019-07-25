
#include "TransformFeedbackImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/TransformFeedback.h>


using namespace gl;


namespace globjects 
{

GLuint TransformFeedbackImplementation_Legacy::create() const
{
    gl::GLuint id;
    glGenTransformFeedbacks(1, &id);

    return id;
}

void TransformFeedbackImplementation_Legacy::destroy(const GLuint id) const
{
    glDeleteTransformFeedbacks(1, &id);
}


} // namespace globjects
