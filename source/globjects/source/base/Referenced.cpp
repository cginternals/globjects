#include <globjects/base/Referenced.h>

namespace glo
{

Referenced::Referenced()
: m_refCounter(0)
{
}

Referenced::~Referenced()
{
}

void Referenced::ref() const
{
	++m_refCounter;
}

void Referenced::unref() const
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

} // namespace glo
