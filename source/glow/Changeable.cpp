#include <cassert>

#include <glow/Changeable.h>
#include <glow/ChangeListener.h>

namespace glow
{

void Changeable::changed()
{
	for (ChangeListener * listener: m_listeners)
	{
		listener->notifyChanged();
		listener->notifyChanged(this);
	}
}

void Changeable::registerListener(ChangeListener * listener)
{
    assert(listener != nullptr);

	m_listeners.insert(listener);
}

void Changeable::deregisterListener(ChangeListener * listener)
{
    assert(listener != nullptr);

	m_listeners.erase(listener);
}

} // namespace glow
