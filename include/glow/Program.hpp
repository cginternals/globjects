#pragma once

#include <glow/Log.h>

namespace glow 
{

template<typename T>
void Program::setUniform(const std::string & name, const T & value)
{
	Uniform<T> * uniform(getUniform<T>(name));
	if (!uniform)
	{
		warning() << "Uniform type mismatch on set uniform. Uniform will be replaced.";

		addUniform(new Uniform<T>(name, value));
		return;
	}
	uniform->set(value);
}

template<typename T>
Uniform<T> * Program::getUniform(const std::string & name)
{
	if (!_uniforms[name])
	{
		Uniform<T> * uniform = new Uniform<T>(name);

		_uniforms[uniform->name()] = uniform;
		uniform->registerProgram(this);

		return uniform;
	}
	return _uniforms[name]->as<T>();
}

} // namespace glow
