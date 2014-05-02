#include <glow/Sampler.h>

#include <glow/Error.h>
#include <glow/ObjectVisitor.h>

namespace glow
{

Sampler::Sampler()
: Object(genSampler())
{
}

Sampler::Sampler(GLuint id, bool takeOwnership)
: Object(id, takeOwnership)
{
}

Sampler * Sampler::fromId(GLuint id, bool takeOwnership)
{
    return new Sampler(id, takeOwnership);
}

Sampler::~Sampler()
{
    if (ownsGLObject())
    {
        glDeleteSamplers(1, &m_id);
        CheckGLError();
    }
}

GLuint Sampler::genSampler()
{
    GLuint id = 0;

    glGenSamplers(1, &id);
    CheckGLError();

    return id;
}

void Sampler::accept(ObjectVisitor & visitor)
{
    visitor.visitSampler(this);
}

void Sampler::bind(GLuint unit) const
{
    glBindSampler(unit, m_id);
    CheckGLError();
}

void Sampler::unbind(GLuint unit)
{
    glBindSampler(unit, 0);
    CheckGLError();
}

void Sampler::setParameter(GLenum name, GLint value)
{
    glSamplerParameteri(m_id, name, value);
    CheckGLError();
}

void Sampler::setParameter(GLenum name, GLfloat value)
{
    glSamplerParameterf(m_id, name, value);
    CheckGLError();
}

GLint Sampler::getParameteri(GLenum pname) const
{
    GLint value = 0;
    glGetSamplerParameteriv(m_id, pname, &value);
    CheckGLError();
	return value;
}

GLfloat Sampler::getParameterf(GLenum pname) const
{
    GLfloat value = 0;
    glGetSamplerParameterfv(m_id, pname, &value);
    CheckGLError();
	return value;
}

} // namespace glow
