#pragma once

#include <glow/Program.h>

#include <cassert>

#include <glow/logging.h>
#include <glow/Uniform.h>
#include <glow/Error.h>
#include <glow/Shader.h>

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
    if (m_uniforms.count(name))
		return m_uniforms[name]->as<T>();

	// create new uniform if none named <name> exists

	Uniform<T> * uniform = new Uniform<T>(name);

	m_uniforms[uniform->name()] = uniform;
	uniform->registerProgram(this);

	return uniform;
}

template <class ...Shaders>
void Program::attach(Shader * shader, Shaders... shaders)
{
    assert(shader != nullptr);

    glAttachShader(m_id, shader->id());
    CheckGLError();

    shader->registerListener(this);
    m_shaders.insert(shader);

    invalidate();

    attach(std::forward<Shaders>(shaders)...);
}

} // namespace glow
