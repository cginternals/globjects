
#include <globjects/base/ChangeListener.h>

#include <cassert>

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
    assert(subject != nullptr);

    const auto it = m_subjects.find(subject);

    if (it == m_subjects.end())
    {
        return;
    }

    m_subjects.erase(it);
    subject->deregisterListener(this);
}


} // namespace globjects
