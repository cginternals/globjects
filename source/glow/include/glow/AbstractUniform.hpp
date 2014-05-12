#pragma once

#include <glow/AbstractUniform.h>

namespace glow 
{

template<typename T>
Uniform<T> * AbstractUniform::as()
{
	return dynamic_cast<Uniform<T>*>(this);
}

template<typename T>
const Uniform<T> * AbstractUniform::as() const
{
    return dynamic_cast<const Uniform<T>*>(this);
}


template <typename T, std::size_t Count>
void AbstractUniform::setValue(const Program * program, gl::GLint location, const std::array<T, Count> & value) const
{
    setValue(program, location, std::vector<T>(value.data(), value.data()+Count));
}

} // namespace glow
