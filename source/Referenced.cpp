#include <glow/Referenced.h>

using namespace glow;

Referenced::Referenced()
: _refCounter(0)
{
}

Referenced::~Referenced()
{
}

void Referenced::ref()
{
	_refCounter++;
}

void Referenced::unref()
{
	_refCounter--;
}

int Referenced::refCounter() const
{
	return _refCounter;
}
