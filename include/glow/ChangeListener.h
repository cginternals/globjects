#pragma once

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
public:
    virtual void notifyChanged();
    virtual void notifyChanged(Changeable* sender);
};

} // namespace glow
