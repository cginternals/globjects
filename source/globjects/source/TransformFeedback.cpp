#include <globjects/TransformFeedback.h>

#include <cassert>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/ObjectVisitor.h>
#include <globjects/Program.h>

#include "Resource.h"


using namespace gl;

namespace globjects
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

void TransformFeedback::bind() const
{
    bind(GL_TRANSFORM_FEEDBACK);
}

void TransformFeedback::unbind()
{
    unbind(GL_TRANSFORM_FEEDBACK);
}

void TransformFeedback::bind(const GLenum target) const
{
    glBindTransformFeedback(target, id());
}

void TransformFeedback::unbind(const GLenum target)
{
    glBindTransformFeedback(target, 0);
}

void TransformFeedback::begin(const GLenum primitiveMode)
{
	glBeginTransformFeedback(primitiveMode);
}

void TransformFeedback::pause()
{
    glPauseTransformFeedback();
}

void TransformFeedback::resume()
{
    glResumeTransformFeedback();
}

void TransformFeedback::end()
{
	glEndTransformFeedback();
}

void TransformFeedback::draw(const GLenum primitiveMode) const
{
    bind(GL_TRANSFORM_FEEDBACK); // TODO: is this necessary

    glDrawTransformFeedback(primitiveMode, id());
}

void TransformFeedback::setVaryings(const Program * program, const GLsizei count, const char** varyingNames, const GLenum bufferMode)
{
    assert(varyingNames != nullptr || count == 0);

    bind(GL_TRANSFORM_FEEDBACK);

    glTransformFeedbackVaryings(program->id(), count, varyingNames, bufferMode);

	program->invalidate();
}

void TransformFeedback::setVaryings(const Program * program, const std::vector<std::string> & varyingNames, GLenum bufferMode)
{
    std::vector<const char*> c_ptrs;
    c_ptrs.reserve((varyingNames.size()));

    for (auto & name : varyingNames)
    {
        c_ptrs.push_back(name.data());
    }

    setVaryings(program, static_cast<GLint>(varyingNames.size()), const_cast<const char**>(c_ptrs.data()), bufferMode);
}

bool TransformFeedback::isTransformFeedback(const GLuint id)
{
    return glIsTransformFeedback(id) == GL_TRUE;
}

GLenum TransformFeedback::objectType() const
{
    return GL_TRANSFORM_FEEDBACK;
}

} // namespace globjects
