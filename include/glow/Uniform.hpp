#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <glow/Error.h>
#include <glow/Array.h>
#include <glow/Texture.h>
#include <glow/Uniform.h>

namespace glow
{

class TemplateError 
{
	private:
		TemplateError();
};


template<typename T>
Uniform<T>::Uniform(const std::string & name)
:	Uniform(name, T())
{
}

template<typename T>
Uniform<T>::Uniform(const std::string & name, const T & value)
:	AbstractUniform(name)
,	m_value(value)
{
}

template<typename T>
Uniform<T>::~Uniform()
{
}

template<typename T>
const T & Uniform<T>::value() const
{
	return m_value;
}

template<typename T>
void Uniform<T>::setLocation(GLint location)
{
	set(location, m_value);
}

template<typename T>
void Uniform<T>::set(const T & value)
{
	m_value = value;
	changed();
}

template<typename T>
void Uniform<T>::set(GLint location, const T & value)
{
    UniformSetter::set(location, value);
}

} // namespace glow
