#include <glow/TransformFeedback.h>

#include <cassert>

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
		glDeleteTransformFeedbacks(1, &m_id);
		CheckGLError();
	}
}

GLuint TransformFeedback::genTransformFeedback()
{
    GLuint id = 0;

	glGenTransformFeedbacks(1, &id);
	CheckGLError();

	return id;
}

void TransformFeedback::accept(ObjectVisitor& visitor)
{
	visitor.visitTransformFeedback(this);
}

void TransformFeedback::bind(GLenum target) const
{
    glBindTransformFeedback(target, m_id);
	CheckGLError();
}

void TransformFeedback::unbind(GLenum target)
{
    glBindTransformFeedback(target, 0);
	CheckGLError();
}

void TransformFeedback::begin(GLenum primitiveMode)
{
	glBeginTransformFeedback(primitiveMode);
	CheckGLError();
}

void TransformFeedback::pause()
{
	glPauseTransformFeedback();
	CheckGLError();
}

void TransformFeedback::resume()
{
	glResumeTransformFeedback();
	CheckGLError();
}

void TransformFeedback::end()
{
	glEndTransformFeedback();
	CheckGLError();
}

void TransformFeedback::draw(GLenum primitiveMode) const
{
    bind(GL_TRANSFORM_FEEDBACK); // TODO: is this necessary

    glDrawTransformFeedback(primitiveMode, m_id);
	CheckGLError();
}

void TransformFeedback::setVaryings(const Program * program, GLsizei count, const char** varyingNames, GLenum bufferMode) const
{
    assert(varyingNames != nullptr || count == 0);

    bind(GL_TRANSFORM_FEEDBACK);

	glTransformFeedbackVaryings(program->id(), count, varyingNames, bufferMode);
	CheckGLError();

	program->invalidate();
}

void TransformFeedback::setVaryings(const Program *program, const std::vector<const char*> & varyingNames, GLenum bufferMode) const
{
    setVaryings(program, static_cast<GLint>(varyingNames.size()), const_cast<const char**>(varyingNames.data()), bufferMode);
}

bool TransformFeedback::isTransformFeedback(GLuint id)
{
    bool result = glIsTransformFeedback(id) == GL_TRUE;
	CheckGLError();

	return result;
}

} // namespace glow
