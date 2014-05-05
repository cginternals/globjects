#include <glowbase/Referenced.h>

namespace glow
{

Referenced::Referenced()
: m_refCounter(0)
{
}

Referenced::Referenced(const Referenced&)
: m_refCounter(0)
{
}


Referenced::~Referenced()
{
}

Referenced& Referenced::operator=(const Referenced&)
{
	m_refCounter = 0;
	return *this;
}

void Referenced::ref()
{
	++m_refCounter;
}

void Referenced::unref()
{
	--m_refCounter;

	if (m_refCounter <= 0)
	{
        destroy();
	}
}

int Referenced::refCounter() const
{
	return m_refCounter;
}

} // namespace glowbase
