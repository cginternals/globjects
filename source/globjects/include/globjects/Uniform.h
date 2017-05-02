
#pragma once

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>
#include <globjects/AbstractUniform.h>
#include <globjects/base/Instantiator.h>


namespace globjects
{


/** \brief Wraps access to typed global GLSL variables.
 *
 * The Uniform class wraps access to typed global GLSL variables (uniforms).
 * These are stored in the OpenGL program objects itself.
 *
 * Supported OpenGL uniform setters are wrapped via specialized template set
 * implementations. Note that unsupported uniform types result in compile time
 * errors due to the default implementation of set.
 *
 * Simple usage of an Uniform:
 * \code{.cpp}
 * std::unique_ptr<Uniform<float>> u = Uniform<float>::create("u_ratio");
 * u->set(1.618f);
 *
 * program->addUniform(u.get());
 * \endcode
 *
 * \see AbstractUniform
 * \see Program
 * \see http://www.opengl.org/wiki/Uniform
 */
template<typename T>
class GLOBJECTS_TEMPLATE_API Uniform : public AbstractUniform, public Instantiator<Uniform<T>>
{
public:
    Uniform(const Program * program, gl::GLint location);
    Uniform(const Program * program, gl::GLint location, const T & value);
    Uniform(const Program * program, const std::string & name);
    Uniform(const Program * program, const std::string & name, const T & value);

    virtual ~Uniform();

    void set(const T & value);

    const T & value() const;

protected:
    virtual void updateAt(gl::GLint location) const override;

protected:
    T m_value; ///< The uniforms value, explictly required when relinking programs.
};


} // namespace globjects


#include <globjects/Uniform.inl>
