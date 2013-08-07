#pragma once

#include <glow/LoggingInterface.h>

namespace glow {

class ConsoleLogger : public LoggingInterface
{
public:
	void handle(const LogMessage& message) override;
protected:
	std::string levelString(LogMessage::Level level);
};

} // namespace glow
