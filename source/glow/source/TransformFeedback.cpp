#include <glow/TransformFeedback.h>

#include <cassert>

#include <glbinding/functions.h>

#include <glow/ObjectVisitor.h>
#include <glow/Program.h>

#include "Resource.h"

namespace glow
{

TransformFeedback::TransformFeedback()
: Object(new TransformFeedbackResource)
{
}

TransformFeedback::~TransformFeedback()
{
}

void TransformFeedback::accept(ObjectVisitor& visitor)
{
	visitor.visitTransformFeedback(this);
}

void TransformFeedback::bind(gl::GLenum target) const
{
    gl::glBindTransformFeedback(target, id());
}

void TransformFeedback::unbind(gl::GLenum target)
{
    gl::glBindTransformFeedback(target, 0);
}

void TransformFeedback::begin(gl::GLenum primitiveMode)
{
	gl::glBeginTransformFeedback(primitiveMode);
}

void TransformFeedback::pause()
{
    gl::glPauseTransformFeedback();
}

void TransformFeedback::resume()
{
    gl::glResumeTransformFeedback();
}

void TransformFeedback::end()
{
	gl::glEndTransformFeedback();
}

void TransformFeedback::draw(gl::GLenum primitiveMode) const
{
    bind(gl::GL_TRANSFORM_FEEDBACK); // TODO: is this necessary

    gl::glDrawTransformFeedback(primitiveMode, id());
}

void TransformFeedback::setVaryings(const Program * program, gl::GLsizei count, const char** varyingNames, gl::GLenum bufferMode) const
{
    assert(varyingNames != nullptr || count == 0);

    bind(gl::GL_TRANSFORM_FEEDBACK);

    gl::glTransformFeedbackVaryings(program->id(), count, varyingNames, bufferMode);

	program->invalidate();
}

void TransformFeedback::setVaryings(const Program *program, const std::vector<const char*> & varyingNames, gl::GLenum bufferMode) const
{
    setVaryings(program, static_cast<gl::GLint>(varyingNames.size()), const_cast<const char**>(varyingNames.data()), bufferMode);
}

bool TransformFeedback::isTransformFeedback(gl::GLuint id)
{
    return gl::glIsTransformFeedback(id) == gl::GL_TRUE;
}

} // namespace glow
