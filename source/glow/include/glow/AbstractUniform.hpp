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

} // namespace glow
