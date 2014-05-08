#pragma once



#include <glbinding/constants.h>

#include <glow/glow_api.h>
#include <glow/Object.h>

namespace glow 
{

class ObjectVisitor;

/** \brief Encapsulates OpenGL render buffer objects.
 
    \see http://www.opengl.org/wiki/Renderbuffer_Objects
 */
class GLOW_API RenderBufferObject : public Object
{
public:
    RenderBufferObject();

    virtual void accept(ObjectVisitor & visitor) override;

    void bind(gl::GLenum target = gl::RENDERBUFFER) const;
    static void unbind(gl::GLenum target = gl::RENDERBUFFER);

    void storage(gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height);
    void storageMultisample(gl::GLsizei samples, gl::GLenum internalformat, gl::GLsizei width, gl::GLsizei height);

    gl::GLint getParameter(gl::GLenum pname) const;
protected:
    virtual ~RenderBufferObject();

	static gl::GLuint genRenderBuffer();
};

} // namespace glow
