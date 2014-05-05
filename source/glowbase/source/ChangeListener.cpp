#include <glowbase/ChangeListener.h>

#include <glowbase/Changeable.h>

namespace glow
{

ChangeListener::~ChangeListener()
{
    for (Changeable * subject : m_subjects)
    {
        subject->deregisterListener(this);
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

} // namespace glowbase
