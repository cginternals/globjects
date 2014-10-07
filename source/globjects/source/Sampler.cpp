#include <globjects/Sampler.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/ObjectVisitor.h>

#include "Resource.h"


using namespace gl;

namespace globjects
{

Sampler::Sampler()
: Object(new SamplerResource)
{
}

Sampler::Sampler(IDResource * resource)
: Object(resource)
{
}

Sampler * Sampler::fromId(GLuint id)
{
    return new Sampler(new ExternalResource(id));
}

Sampler::~Sampler()
{
}

void Sampler::accept(ObjectVisitor & visitor)
{
    visitor.visitSampler(this);
}

void Sampler::bind(GLuint unit) const
{
    glBindSampler(unit, id());
}

void Sampler::unbind(GLuint unit)
{
    glBindSampler(unit, 0);
}

void Sampler::setParameter(GLenum name, GLint value)
{
    glSamplerParameteri(id(), name, value);
}

void Sampler::setParameter(GLenum name, GLfloat value)
{
    glSamplerParameterf(id(), name, value);
}

GLint Sampler::getParameteri(GLenum pname) const
{
    GLint value = 0;
    glGetSamplerParameteriv(id(), pname, &value);

	return value;
}

GLfloat Sampler::getParameterf(GLenum pname) const
{
    GLfloat value = 0;
    glGetSamplerParameterfv(id(), pname, &value);

	return value;
}

GLenum Sampler::objectType() const
{
    return GL_SAMPLER;
}

} // namespace globjects
