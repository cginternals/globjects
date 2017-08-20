
#include <globjects/Sampler.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/ObjectVisitor.h>

#include <globjects/Resource.h>

using namespace gl;


namespace globjects
{


Sampler::Sampler()
: Object(std::unique_ptr<IDResource>(new SamplerResource))
{
}

Sampler::Sampler(std::unique_ptr<IDResource> && resource)
: Object(std::move(resource))
{
}

std::unique_ptr<Sampler> Sampler::fromId(const GLuint id)
{
    return std::unique_ptr<Sampler>(new Sampler(std::unique_ptr<IDResource>(new ExternalResource(id))));
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

void Sampler::setParameter(gl::GLenum name, const glm::vec4 & value)
{
    glSamplerParameterfv(id(), name, glm::value_ptr(value));
}

void Sampler::setParameter(gl::GLenum name, const glm::ivec4 & value)
{
    glSamplerParameterIiv(id(), name, glm::value_ptr(value));
}

void Sampler::setParameter(gl::GLenum name, const glm::uvec4 & value)
{
    glSamplerParameterIuiv(id(), name, glm::value_ptr(value));
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
