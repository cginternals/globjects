#pragma once

#include <GL/glew.h>

#include <glow/glow.h>
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
    Sampler(GLuint id, bool ownsGLObject = true);
    virtual ~Sampler();

    virtual void accept(ObjectVisitor & visitor) override;

    void bind(GLuint unit) const;

    void setParameter(GLenum name, GLint value);
    void setParameter(GLenum name, GLfloat value);

    GLint getParameteri(GLenum pname) const;
    GLfloat getParameterf(GLenum pname) const;

protected:
    static GLuint genSampler();

};

} // namespace glow
