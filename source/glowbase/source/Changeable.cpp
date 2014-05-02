#include <glowbase/Changeable.h>

#include <cassert>

#include <glowbase/ChangeListener.h>

namespace glowbase
{

void Changeable::changed() const
{
	for (ChangeListener * listener: m_listeners)
	{
		listener->notifyChanged(this);
	}
}

void Changeable::registerListener(ChangeListener * listener)
{
    assert(listener != nullptr);

	m_listeners.insert(listener);
    listener->addSubject(this);
}

void Changeable::deregisterListener(ChangeListener * listener)
{
    assert(listener != nullptr);

    if (m_listeners.find(listener) == m_listeners.end())
        return;

	m_listeners.erase(listener);
    listener->removeSubject(this);
}

} // namespace glowbase