#pragma once

#include <string>
#include <set>

#include <GL/glew.h>

#include <glow/glow_api.h>
#include <glow/Referenced.h>
#include <glow/LocationIdentity.h>

namespace glow
{

class Program;
template<typename T> class Uniform;

/** \brief Abstract base class for templated Uniforms.
 *
 * Unifies the specialized Uniforms in order to be able to store them in a list or a vector.
 *
 * \see Uniform
 * \see Program
 */
class GLOW_API AbstractUniform : public Referenced
{
	friend class Program; ///< Programs (de)register themselves.

public:
    AbstractUniform(GLint location);
	AbstractUniform(const std::string & name);

	const std::string & name() const;
    GLint location() const;

    const LocationIdentity & identity() const;

    /** Simplifies the often required casting of AbstractUniforms.
     *
     * @return a specialized Uniform of the requested type, returns a nullptr on a type mismatch
     *
     * \code{.cpp}
     * abstractUniform->as<float>()->set(3.142f);
     * \endcode
	*/
	template<typename T> Uniform<T> * as();
    template<typename T> const Uniform<T> * as() const;

protected:
    virtual ~AbstractUniform();

	void registerProgram(Program * program);
	void deregisterProgram(Program * program);

	/** Iterates over all programs attached to and calls update.
		Should be called on every value change (i.e., in Uniform).
	*/
	void changed();

	/** Sets the uniform's value on the program.
	*/
    void update(const Program * program) const;

	/** This function requires knowledge of the unifom's value.
	*/
    virtual void setValueAt(GLint location) const = 0;
    virtual void setValueAt(const Program* program, GLint location) const = 0;

    GLint locationFor(const Program * program) const;
protected:
    LocationIdentity m_identity;
	std::set<Program *> m_programs;
    mutable bool m_directStateAccess;
    mutable bool m_dsaCached;
};

} // namespace glow

#include <glow/AbstractUniform.hpp>
