
#include <glow/Changeable.h>

namespace glow
{

void Changeable::changed()
{
	for (ChangeListener * listener: m_listeners)
	{
		listener->notifyChanged();
	}
}

void Changeable::registerListener(ChangeListener * listener)
{
	m_listeners.insert(listener);
}

void Changeable::deregisterListener(ChangeListener * listener)
{
	m_listeners.erase(listener);
}

} // namespace glow
