#pragma once

#include <string>

#include <glow/glow.h>

#include <glow/LogMessage.h>


namespace glow {

class GLOW_API LoggingInterface
{
public:
	virtual void handle(const LogMessage& message) = 0;
};

} // namespace glow
