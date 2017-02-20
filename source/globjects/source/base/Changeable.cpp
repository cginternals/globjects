
#include <globjects/base/Changeable.h>

#include <cassert>

#include <globjects/base/ChangeListener.h>


namespace globjects
{


Changeable::Changeable()
{
}

Changeable::~Changeable()
{
    while (!m_listeners.empty())
    {
        // calls deregisterListener
        (*m_listeners.begin())->removeSubject(this);
    }
}

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

    const auto it = m_listeners.find(listener);

    if (it == m_listeners.end())
    {
        return;
    }

    m_listeners.erase(it);
    listener->removeSubject(this);
}


} // namespace globjects
