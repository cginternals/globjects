
#pragma once


#include <memory>

#include "../base/Singleton.h"

#include "AbstractRenderbufferImplementation.h"


namespace globjects
{


class RenderbufferImplementation_Legacy : public AbstractRenderbufferImplementation
    , public Singleton<RenderbufferImplementation_Legacy>
{
public:
    virtual gl::GLuint create() const override;
    virtual void destroy(gl::GLuint id) const override;

    virtual void bind(const Renderbuffer * renderbuffer, gl::GLenum target) const override;
    virtual void unbind(gl::GLenum target) const override;
    virtual void storage(const Renderbuffer * renderbuffer, gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height) const override;
    virtual void storageMultisample(const Renderbuffer * renderbuffer, gl::GLsizei samples, gl::GLenum internalformat, gl::GLsizei width, const gl::GLsizei height) const override;
    virtual gl::GLint getParameterInt(const Renderbuffer * renderbuffer, gl::GLenum pname) const override;
};


} // namespace globjects
