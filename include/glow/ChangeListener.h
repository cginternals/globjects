#pragma once

#include <glow/declspec.h>

namespace glow {

class GLOW_API ChangeListener
{
public:
	virtual void notifyChanged() = 0;
};

} // namespace glow
