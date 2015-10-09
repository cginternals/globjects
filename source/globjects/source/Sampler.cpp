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

Sampler * Sampler::fromId(const GLuint id)
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

void Sampler::bind(const GLuint unit) const
{
    glBindSampler(unit, id());
}

void Sampler::unbind(const GLuint unit)
{
    glBindSampler(unit, 0);
}

void Sampler::setParameter(const GLenum name, const GLenum value)
{
    glSamplerParameteri(id(), name, static_cast<GLint>(value));
}

void Sampler::setParameter(const GLenum name, const GLint value)
{
    glSamplerParameteri(id(), name, value);
}

void Sampler::setParameter(const GLenum name, const GLfloat value)
{
    glSamplerParameterf(id(), name, value);
}

GLint Sampler::getParameteri(const GLenum pname) const
{
    GLint value = 0;
    glGetSamplerParameteriv(id(), pname, &value);

	return value;
}

GLfloat Sampler::getParameterf(const GLenum pname) const
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
