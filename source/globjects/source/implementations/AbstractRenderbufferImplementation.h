
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

    virtual void bind(const Renderbuffer * renderbuffer, gl::GLenum target) const = 0;
    virtual void unbind(gl::GLenum target) const = 0;
    virtual void storage(const Renderbuffer * renderbuffer, gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height) const = 0;
    virtual void storageMultisample(const Renderbuffer * renderbuffer, gl::GLsizei samples, gl::GLenum internalformat, gl::GLsizei width, const gl::GLsizei height) const = 0;
    virtual gl::GLint getParameterInt(const Renderbuffer * renderbuffer, gl::GLenum pname) const = 0;
};


} // namespace globjects
