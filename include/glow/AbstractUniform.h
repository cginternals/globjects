#pragma once

#include <string>
#include <set>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>

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
	AbstractUniform(const std::string & name);
	virtual ~AbstractUniform();

	const std::string & name() const;

    /** Simplifies the often required casting of AbstractUniforms.
     *
     * @return a specialized Uniform of the requested type, returns a nullptr on a type mismatch
     *
     * \code{.cpp}
     * abstractUniform->as<float>()->set(3.142f);
     * \endcode
	*/
	template<typename T> Uniform<T> * as();

protected:
	void registerProgram(Program * program);
	void deregisterProgram(Program * program);

	/** Iterates over all programs attached to and calls update.
		Should be called on every value change (i.e., in Uniform).
	*/
	void changed();

	/** Sets the uniform's value on the program.
	*/
	void update(Program * program);

	/** This function requires knowledge of the unifom's value.
	*/
	virtual void setLocation(GLint location) = 0;

protected:
	std::string m_name;
	std::set<Program *> m_programs;
};

} // namespace glow

#include <glow/AbstractUniform.hpp>
