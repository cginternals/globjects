
#pragma once

#include <glm/fwd.hpp>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>
#include <globjects/Object.h>
#include <globjects/base/Instantiator.h>


namespace globjects 
{


/** \brief Wraps OpenGL sampler objects.
        
    \see http://www.opengl.org/wiki/Sampler_Object
 */
class GLOBJECTS_API Sampler : public Object, public Instantiator<Sampler>
{
public:
    Sampler();

    virtual ~Sampler();

    static std::unique_ptr<Sampler> fromId(gl::GLuint id);

    virtual void accept(ObjectVisitor & visitor) override;

    void bind(gl::GLuint unit) const;
    static void unbind(gl::GLuint unit);

    void setParameter(gl::GLenum name, gl::GLenum value);
    void setParameter(gl::GLenum name, gl::GLint value);
    void setParameter(gl::GLenum name, gl::GLfloat value);
    void setParameter(gl::GLenum name, const glm::vec4 & value);
    void setParameter(gl::GLenum name, const glm::ivec4 & value);
    void setParameter(gl::GLenum name, const glm::uvec4 & value);

    gl::GLint getParameteri(gl::GLenum pname) const;
    gl::GLfloat getParameterf(gl::GLenum pname) const;

    virtual gl::GLenum objectType() const override;

protected:
    Sampler(std::unique_ptr<IDResource> && resource);
};


} // namespace globjects
