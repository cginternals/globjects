#pragma once

#include <set>

#include <glowbase/glowbase_api.h>

namespace glow
{

class ChangeListener;

/** \brief Superclass of all objects that want others to signal that they have changed.
    
    It implements the observer pattern. Listeners to the subclass change can be
    registered using registerListener() and deregistered using deregisterListener().
    
    \see ChangeListener
 */
class GLOWBASE_API Changeable
{
public:
    void changed() const;

	void registerListener(ChangeListener * listener);
	void deregisterListener(ChangeListener * listener);

private:
    std::set<ChangeListener *> m_listeners;
};

} // namespace glowbase
