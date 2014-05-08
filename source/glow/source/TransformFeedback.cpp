#include <glow/TransformFeedback.h>

#include <cassert>

#include <glbinding/functions.h>

#include <glow/Error.h>
#include <glow/ObjectVisitor.h>
#include <glow/Program.h>

namespace glow
{

TransformFeedback::TransformFeedback()
: Object(genTransformFeedback())
{
}

TransformFeedback::~TransformFeedback()
{
	if (ownsGLObject())
	{
		gl::DeleteTransformFeedbacks(1, &m_id);
		CheckGLError();
	}
}

gl::GLuint TransformFeedback::genTransformFeedback()
{
    gl::GLuint id = 0;

	gl::GenTransformFeedbacks(1, &id);
	CheckGLError();

	return id;
}

void TransformFeedback::accept(ObjectVisitor& visitor)
{
	visitor.visitTransformFeedback(this);
}

void TransformFeedback::bind(gl::GLenum target) const
{
    gl::BindTransformFeedback(target, m_id);
	CheckGLError();
}

void TransformFeedback::unbind(gl::GLenum target)
{
    gl::BindTransformFeedback(target, 0);
	CheckGLError();
}

void TransformFeedback::begin(gl::GLenum primitiveMode)
{
	gl::BeginTransformFeedback(primitiveMode);
	CheckGLError();
}

void TransformFeedback::pause()
{
    gl::PauseTransformFeedback();
	CheckGLError();
}

void TransformFeedback::resume()
{
    gl::ResumeTransformFeedback();
	CheckGLError();
}

void TransformFeedback::end()
{
	gl::EndTransformFeedback();
	CheckGLError();
}

void TransformFeedback::draw(gl::GLenum primitiveMode) const
{
    bind(gl::TRANSFORM_FEEDBACK); // TODO: is this necessary

    gl::DrawTransformFeedback(primitiveMode, m_id);
	CheckGLError();
}

void TransformFeedback::setVaryings(const Program * program, gl::GLsizei count, const char** varyingNames, gl::GLenum bufferMode) const
{
    assert(varyingNames != nullptr || count == 0);

    bind(gl::TRANSFORM_FEEDBACK);

    gl::TransformFeedbackVaryings(program->id(), count, varyingNames, bufferMode);
	CheckGLError();

	program->invalidate();
}

void TransformFeedback::setVaryings(const Program *program, const std::vector<const char*> & varyingNames, gl::GLenum bufferMode) const
{
    setVaryings(program, static_cast<gl::GLint>(varyingNames.size()), const_cast<const char**>(varyingNames.data()), bufferMode);
}

bool TransformFeedback::isTransformFeedback(gl::GLuint id)
{
    bool result = gl::IsTransformFeedback(id) == gl::TRUE;
	CheckGLError();

	return result;
}

} // namespace glow
