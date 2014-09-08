#pragma once

#include <set>

#include <globjects/globjects_api.h>

namespace globjects
{

class ChangeListener;

/** \brief Superclass of all objects that want others to signal that they have changed.
    
    It implements the observer pattern. Listeners to the subclass change can be
    registered using registerListener() and deregistered using deregisterListener().
    
    \see ChangeListener
 */
class GLOBJECTS_API Changeable
{
public:
    void changed() const;

	void registerListener(ChangeListener * listener);
	void deregisterListener(ChangeListener * listener);

private:
    std::set<ChangeListener *> m_listeners;
};

} // namespace globjects
