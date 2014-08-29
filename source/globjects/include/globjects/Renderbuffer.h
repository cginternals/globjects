#pragma once

#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>

#include <globjects/globjects_api.h>
#include <globjects/Object.h>

namespace glo 
{

class ObjectVisitor;

/** \brief Encapsulates OpenGL render buffer objects.
 
    \see http://www.opengl.org/wiki/Renderbuffer_Objects
 */
class GLOBJECTS_API Renderbuffer : public Object
{
public:
    Renderbuffer();

    virtual void accept(ObjectVisitor & visitor) override;

    void bind(gl::GLenum target = gl::GL_RENDERBUFFER) const;
    static void unbind(gl::GLenum target = gl::GL_RENDERBUFFER);

    void storage(gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height);
    void storageMultisample(gl::GLsizei samples, gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height);

    gl::GLint getParameter(gl::GLenum pname) const;

    virtual gl::GLenum objectType() const override;

protected:
    virtual ~Renderbuffer();
};

} // namespace glo
