
#pragma once


#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/Sampler.h>


namespace globjects
{


class AbstractSamplerImplementation
{
public:
    AbstractSamplerImplementation();
    virtual ~AbstractSamplerImplementation();

    static AbstractSamplerImplementation * get(Sampler::Implementation impl =
        Sampler::Implementation::Legacy);

    virtual gl::GLuint create() const = 0;
    virtual void destroy(gl::GLuint id) const = 0;
};


} // namespace globjects
