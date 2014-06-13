#include <glow/Sampler.h>

#include <glbinding/functions.h>

#include <glow/ObjectVisitor.h>

namespace glow
{

Sampler::Sampler()
: Object(genSampler())
{
}

Sampler::Sampler(gl::GLuint id, bool takeOwnership)
: Object(id, takeOwnership)
{
}

Sampler * Sampler::fromId(gl::GLuint id, bool takeOwnership)
{
    return new Sampler(id, takeOwnership);
}

Sampler::~Sampler()
{
    if (ownsGLObject())
    {
        gl::DeleteSamplers(1, &m_id);
    }
}

gl::GLuint Sampler::genSampler()
{
    gl::GLuint id = 0;

    gl::GenSamplers(1, &id);

    return id;
}

void Sampler::accept(ObjectVisitor & visitor)
{
    visitor.visitSampler(this);
}

void Sampler::bind(gl::GLuint unit) const
{
    gl::BindSampler(unit, m_id);
}

void Sampler::unbind(gl::GLuint unit)
{
    gl::BindSampler(unit, 0);
}

void Sampler::setParameter(gl::GLenum name, gl::GLint value)
{
    gl::SamplerParameteri(m_id, name, value);
}

void Sampler::setParameter(gl::GLenum name, gl::GLfloat value)
{
    gl::SamplerParameterf(m_id, name, value);
}

gl::GLint Sampler::getParameteri(gl::GLenum pname) const
{
    gl::GLint value = 0;
    gl::GetSamplerParameteriv(m_id, pname, &value);

	return value;
}

gl::GLfloat Sampler::getParameterf(gl::GLenum pname) const
{
    gl::GLfloat value = 0;
    gl::GetSamplerParameterfv(m_id, pname, &value);

	return value;
}

} // namespace glow
