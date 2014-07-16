#include <glow/Sampler.h>

#include <glbinding/gl/functions.h>

#include <glow/ObjectVisitor.h>

#include "Resource.h"

namespace glow
{

Sampler::Sampler()
: Object(new SamplerResource)
{
}

Sampler::Sampler(IDResource * resource)
: Object(resource)
{
}

Sampler * Sampler::fromId(gl::GLuint id)
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

void Sampler::bind(gl::GLuint unit) const
{
    gl::glBindSampler(unit, id());
}

void Sampler::unbind(gl::GLuint unit)
{
    gl::glBindSampler(unit, 0);
}

void Sampler::setParameter(gl::GLenum name, gl::GLint value)
{
    gl::glSamplerParameteri(id(), name, value);
}

void Sampler::setParameter(gl::GLenum name, gl::GLfloat value)
{
    gl::glSamplerParameterf(id(), name, value);
}

gl::GLint Sampler::getParameteri(gl::GLenum pname) const
{
    gl::GLint value = 0;
    gl::glGetSamplerParameteriv(id(), pname, &value);

	return value;
}

gl::GLfloat Sampler::getParameterf(gl::GLenum pname) const
{
    gl::GLfloat value = 0;
    gl::glGetSamplerParameterfv(id(), pname, &value);

	return value;
}

} // namespace glow
