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
	return _uniforms[name]->as<T>();
}

} // namespace glow
