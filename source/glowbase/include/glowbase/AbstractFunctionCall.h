#pragma once

#include <glowbase/glowbase_api.h>

namespace glow
{

class GLOWBASE_API AbstractFunctionCall
{
public:
	AbstractFunctionCall();
	virtual ~AbstractFunctionCall();

	virtual void operator()() = 0;
	virtual void * identifier() const = 0;

};

} // namespace glowbase
