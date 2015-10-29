#pragma once

#include <globjects/Uniform.h>

namespace globjects
{

template<typename T>
Uniform<T>::Uniform(gl::GLint location)
: Uniform(location, T())
{
}

template<typename T>
Uniform<T>::Uniform(gl::GLint location, const T & value)
: AbstractUniform(location)
, m_value(value)
{
}

template<typename T>
Uniform<T>::Uniform(const std::string & name)
: Uniform(name, T())
{
}

template<typename T>
Uniform<T>::Uniform(const std::string & name, const T & value)
: AbstractUniform(name)
, m_value(value)
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
void Uniform<T>::updateAt(const Program * program, gl::GLint location) const
{
    if (location < 0)
    {
        return;
    }

    setValue(program, location, m_value);
}

template<typename T>
void Uniform<T>::set(const T & value)
{
	m_value = value;
	changed();
}

} // namespace globjects
