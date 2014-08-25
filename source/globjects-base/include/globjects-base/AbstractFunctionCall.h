#pragma once

#include <globjects-base/globjects-base_api.h>

namespace glo
{

class GLOBJECTS_BASE_API AbstractFunctionCall
{
public:
	AbstractFunctionCall();
	virtual ~AbstractFunctionCall();

	virtual void operator()() = 0;
	virtual void * identifier() const = 0;

};

} // namespace glo
