#pragma once

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>
#include <globjects/Object.h>

namespace globjects 
{

/** \brief Wraps OpenGL sampler objects.
        
    \see http://www.opengl.org/wiki/Sampler_Object
 */
class GLOBJECTS_API Sampler : public Object
{
public:
    Sampler();
    static Sampler * fromId(gl::GLuint id);

    virtual void accept(ObjectVisitor & visitor) override;

    void bind(gl::GLuint unit) const;
    static void unbind(gl::GLuint unit);

    void setParameter(gl::GLenum name, gl::GLenum value);
    void setParameter(gl::GLenum name, gl::GLint value);
    void setParameter(gl::GLenum name, gl::GLfloat value);

    gl::GLint getParameteri(gl::GLenum pname) const;
    gl::GLfloat getParameterf(gl::GLenum pname) const;

    virtual gl::GLenum objectType() const override;

protected:
    Sampler(IDResource * resource);
    virtual ~Sampler();
};

} // namespace globjects
