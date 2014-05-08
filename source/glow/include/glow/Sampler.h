#pragma once

#include <glbinding/types.h>

#include <glow/glow_api.h>
#include <glow/Object.h>

namespace glow 
{

/** \brief Wraps OpenGL sampler objects.
        
    \see http://www.opengl.org/wiki/Sampler_Object
 */
class GLOW_API Sampler : public Object
{
public:
    Sampler();
    static Sampler * fromId(gl::GLuint id, bool takeOwnership = false);

    virtual void accept(ObjectVisitor & visitor) override;

    void bind(gl::GLuint unit) const;
    static void unbind(gl::GLuint unit);

    void setParameter(gl::GLenum name, gl::GLint value);
    void setParameter(gl::GLenum name, gl::GLfloat value);

    gl::GLint getParameteri(gl::GLenum pname) const;
    gl::GLfloat getParameterf(gl::GLenum pname) const;

protected:
    Sampler(gl::GLuint id, bool takeOwnership = true);
    virtual ~Sampler();

    static gl::GLuint genSampler();
};

} // namespace glow
