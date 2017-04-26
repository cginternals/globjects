
#pragma once


namespace globjects
{


template<typename T>
Uniform<T>::Uniform(const Program * program, gl::GLint location)
: Uniform(program, location, T())
{
}

template<typename T>
Uniform<T>::Uniform(const Program * program, gl::GLint location, const T & value)
: AbstractUniform(program, location, UniformTypeHelper<T>::value)
, m_value(value)
{
}

template<typename T>
Uniform<T>::Uniform(const Program * program, const std::string & name)
: Uniform(program, name, T())
{
}

template<typename T>
Uniform<T>::Uniform(const Program * program, const std::string & name, const T & value)
: AbstractUniform(program, name, UniformTypeHelper<T>::value)
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
void Uniform<T>::updateAt(gl::GLint location) const
{
    if (location < 0)
    {
        return;
    }

    setValue(location, m_value);
}

template<typename T>
void Uniform<T>::set(const T & value)
{
    m_value = value;
    changed();
}


} // namespace globjects
