#include <glow/TransformFeedback.h>

namespace glow
{

TransformFeedback::TransformFeedback()
: Object(genTransformFeedback())
{
	
}

TransformFeedback::~TransformFeedback()
{
	if (m_id)
	{
		glDeleteTransformFeedbacks(1, &m_id);
	}
}

GLuint TransformFeedback::genTransformFeedback()
{
	GLuint id;
	
	glGenTransformFeedbacks(1, &id);
	
	return id;
}

void TransformFeedback::bind()
{
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_id);
}

void TransformFeedback::unbind()
{
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void TransformFeedback::begin(GLenum primitiveMode)
{
	bind();
	
	glBeginTransformFeedback(primitiveMode);
}

void TransformFeedback::pause()
{
	bind();
	
	glPauseTransformFeedback();
}

void TransformFeedback::resume()
{
	bind();
	
	glResumeTransformFeedback();
}

void TransformFeedback::end()
{
	bind();
	
	glEndTransformFeedback();
}

void TransformFeedback::draw(GLenum primitiveMode)
{
	bind();
	
	glDrawTransformFeedback(primitiveMode, m_id);
}

void TransformFeedback::setVaryings(Program* program, GLsizei count, const char** varyingNames, GLenum bufferMode)
{
	bind();
	
	glTransformFeedbackVaryings(program->id(), count, varyingNames, bufferMode);
	
	program->invalidate();
}

void TransformFeedback::setVaryings(Program* program, const Array<const char*>& varyingNames, GLenum bufferMode)
{
	setVaryings(program, varyingNames.size(), varyingNames.data(), bufferMode);
}

} // namespace glow
