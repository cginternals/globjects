#pragma once

#include <glow/glow.h>

namespace glow {

class GLOW_API ChangeListener
{
public:
	virtual void notifyChanged() = 0;
};

} // namespace glow
