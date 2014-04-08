#pragma once

#include <set>

#include <glow/glow.h>

namespace glow 
{

class Changeable;

/** \brief Allows listening to any Changeable.
    
    If a Changeable this ChangeListener is registered on signals a change, the 
    notifyChanged() method is called. This class implements the observer pattern.
    
    \see Changeable
 */
class GLOW_API ChangeListener
{
    friend class Changeable;
public:
    virtual ~ChangeListener();

    virtual void notifyChanged(const Changeable * sender);

private:
    std::set<Changeable*> m_subjects;

    void addSubject(Changeable * subject);
    void removeSubject(Changeable * subject);
};

} // namespace glow
