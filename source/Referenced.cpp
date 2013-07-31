
#include <glow/Referenced.h>

namespace glow
{

Referenced::Referenced()
: _refCounter(0)
{
}

Referenced::Referenced(const Referenced&)
: _refCounter(0)
{
}


Referenced::~Referenced()
{
}

Referenced& Referenced::operator=(const Referenced&)
{
	_refCounter = 0;
	return *this;
}

void Referenced::ref()
{
	++_refCounter;
}

void Referenced::unref()
{
	--_refCounter;

	if (_refCounter <= 0)
	{
		delete this;
	}
}

int Referenced::refCounter() const
{
	return _refCounter;
}

} // namespace glow
