#pragma once

#include <glbinding/types.h>

#include <glow/glow_api.h>
#include <glow/AbstractUniform.h>

namespace glow
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
 * Uniform<float> * u = new Uniform<float>("u_ratio");
 * u->set(1.618f);
 *
 * program->addUniform(u);
 * \endcode
 *
 * \see AbstractUniform
 * \see Program
 * \see http://www.opengl.org/wiki/Uniform
 */
template<typename T>
class Uniform : public AbstractUniform
{
public:
    Uniform(gl::GLint location);
    Uniform(gl::GLint location, const T & value);
    Uniform(const std::string & name);
    Uniform(const std::string & name, const T & value);

    void set(const T & value);

    const T & value() const;

protected:
    virtual ~Uniform();

    virtual void updateAt(const Program * program, gl::GLint location) const override;
protected:
    T m_value; ///< The uniforms value, explictly required when relinking programs.
};

} // namespace glow

#include <glow/Uniform.hpp>
