#pragma once

namespace glow 
{
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
	return dynamic_cast<Uniform<T>*>(_uniforms[name].get());
}

} // namespace glow
