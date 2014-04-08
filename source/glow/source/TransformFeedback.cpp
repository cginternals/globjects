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
	GLuint id;

	glGenTransformFeedbacks(1, &id);
	CheckGLError();

	return id;
}

void TransformFeedback::accept(ObjectVisitor& visitor)
{
	visitor.visitTransformFeedback(this);
}

void TransformFeedback::bind() const
{
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_id);
	CheckGLError();
}

void TransformFeedback::unbind() const
{
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	CheckGLError();
}

void TransformFeedback::begin(GLenum primitiveMode)
{
	//~ bind(); // no autobind?

	glBeginTransformFeedback(primitiveMode);
	CheckGLError();
}

void TransformFeedback::pause()
{
	//~ bind(); // no autobind?

	glPauseTransformFeedback();
	CheckGLError();
}

void TransformFeedback::resume()
{
	//~ bind(); // no autobind?

	glResumeTransformFeedback();
	CheckGLError();
}

void TransformFeedback::end()
{
	//~ bind(); // no autobind?

	glEndTransformFeedback();
	CheckGLError();
}

void TransformFeedback::draw(GLenum primitiveMode) const
{
	bind();

    glDrawTransformFeedback(primitiveMode, m_id);
	CheckGLError();
}

void TransformFeedback::setVaryings(const Program *program, GLsizei count, const char** varyingNames, GLenum bufferMode) const
{
    assert(varyingNames != nullptr || count == 0);

	bind();

	glTransformFeedbackVaryings(program->id(), count, varyingNames, bufferMode);
	CheckGLError();

	program->invalidate();
}

void TransformFeedback::setVaryings(const Program *program, const std::vector<const char*>& varyingNames, GLenum bufferMode) const
{
    setVaryings(program, static_cast<GLint>(varyingNames.size()), const_cast<const char**>(varyingNames.data()), bufferMode);
}

bool TransformFeedback::isTransformFeedback() const
{
	bool result = GL_TRUE == glIsTransformFeedback(m_id);
	CheckGLError();
	return result;
}

} // namespace glow
