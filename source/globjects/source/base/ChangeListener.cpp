#include <globjects/base/ChangeListener.h>

#include <globjects/base/Changeable.h>

namespace globjects
{

ChangeListener::~ChangeListener()
{
    while (!m_subjects.empty())
    {
        // calls removeSubject
        (*m_subjects.begin())->deregisterListener(this);
    }
}

void ChangeListener::notifyChanged(const Changeable *)
{
}

void ChangeListener::addSubject(Changeable * subject)
{
    m_subjects.insert(subject);
}

void ChangeListener::removeSubject(Changeable * subject)
{
    m_subjects.erase(subject);
}

} // namespace globjects
