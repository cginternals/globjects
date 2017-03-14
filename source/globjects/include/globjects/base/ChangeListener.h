
#pragma once

#include <set>

#include <globjects/globjects_api.h>


namespace globjects
{


class Changeable;

/** \brief Allows listening to any Changeable.
    
    If a Changeable this ChangeListener is registered on signals a change, the 
    notifyChanged() method is called. This class implements the observer pattern.
    
    \see Changeable
 */
class GLOBJECTS_API ChangeListener
{
    friend class Changeable;
public:
    virtual ~ChangeListener();

    virtual void notifyChanged(const Changeable * sender);

protected:
    std::set<Changeable*> m_subjects;

    void addSubject(Changeable * subject);
    virtual void removeSubject(Changeable * subject);
};


} // namespace globjects
