#include <cassert>

#include <glow/ChangeListener.h>
#include <glow/Changeable.h>

namespace glow
{

void Changeable::changed()
{
	for (ChangeListener * listener: m_listeners)
		listener->notifyChanged();
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
