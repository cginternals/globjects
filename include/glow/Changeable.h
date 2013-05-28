#pragma once

#include <glow/declspec.h>

#include <glow/ChangeListener.h>
#include <set>

namespace glow {

class GLOW_API Changeable
{
public:
	void changed();

	void registerListener(ChangeListener* listener);
	void deregisterListener(ChangeListener* listener);
protected:
	std::set<ChangeListener*> _listeners;
};

} // namespace glow
