#pragma once

#include <glow/glow.h>
#include <glow/AbstractUniform.h>

namespace glow 
{

template<typename T>
class GLOW_API Uniform : public AbstractUniform
{
public:
	Uniform(const std::string & name);
	virtual ~Uniform();

	void set(const T & value);

protected:
	virtual void update(Program * program);
	void set(
		const int location
	,	const T & value);

protected:
	T _value;
};

} // namespace glow

#include <glow/Uniform.hpp>
