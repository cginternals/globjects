#include <glow/Changeable.h>

using namespace glow;

void Changeable::changed()
{
	for (ChangeListener* listener: _listeners)
	{
		listener->notifyChanged();
	}
}

void Changeable::registerListener(ChangeListener* listener)
{
	_listeners.insert(listener);
}

void Changeable::deregisterListener(ChangeListener* listener)
{
	_listeners.erase(listener);
}
