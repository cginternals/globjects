#pragma once

#include <string>
#include <set>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>

namespace glow
{

template<typename T> class Uniform;
class Program;

/** \brief Unites specialized Uniforms.

	The Uniform class wraps access to global GLSL variables (uniforms). In
	glow, uniforms can be reused in multiple programs. A change in value causes
	a value update in every program the uniform is attached to. It is not
	required to aquire uniform locations when using this class. The value and
	its type are specified through a typed Uniform instance.

	\code{.cpp}

		Uniform<float> * u(new Uniform<float>("u_ratio"));

		u->set(1.618f);
		program->addUniform(u);

	\endcode

	\see Uniform
	\see Program
*/
class GLOW_API AbstractUniform : public Referenced
{
	friend class Program; ///< Programs (de)register themselves.

public:
	AbstractUniform(const std::string & name);
	virtual ~AbstractUniform();

	const std::string & name() const;

	/** Simplyfies the often required casting of AbstractUniforms.

		\retun Note that the dynamic cast that is internally used might
			return a nullptr.

		\code{.cpp}
		abstractUniform->as<float>()->set(3.142f);
		\endcode
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
