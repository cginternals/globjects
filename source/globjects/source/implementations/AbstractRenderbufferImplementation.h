
#pragma once


#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/Renderbuffer.h>


namespace globjects
{


class AbstractRenderbufferImplementation
{
public:
    AbstractRenderbufferImplementation();
    virtual ~AbstractRenderbufferImplementation();

    static AbstractRenderbufferImplementation * get(Renderbuffer::Implementation impl =
        Renderbuffer::Implementation::Legacy);

    virtual gl::GLuint create() const = 0;
    virtual void destroy(gl::GLuint id) const = 0;
};


} // namespace globjects
