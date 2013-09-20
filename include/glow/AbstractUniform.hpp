#pragma once

namespace glow 
{

template<typename T>
Uniform<T> * AbstractUniform::as()
{
	return dynamic_cast<Uniform<T>*>(this);
}

} // namespace glow
