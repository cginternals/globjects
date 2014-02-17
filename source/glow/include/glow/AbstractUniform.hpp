#pragma once

#include <glow/AbstractUniform.h>

namespace glow 
{

template<typename T>
Uniform<T> * AbstractUniform::as()
{
	return dynamic_cast<Uniform<T>*>(this);
}

} // namespace glow
