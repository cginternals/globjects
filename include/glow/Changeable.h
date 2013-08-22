#pragma once

#include <set>

#include <glow/glow.h>
#include <glow/ChangeListener.h>

namespace glow 
{

class GLOW_API Changeable
{
public:
	void changed();

	void registerListener(ChangeListener * listener);
	void deregisterListener(ChangeListener * listener);

protected:
	std::set<ChangeListener *> m_listeners;
};

} // namespace glow
