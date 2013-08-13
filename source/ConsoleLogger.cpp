
#include <iostream>

#include <glow/ConsoleLogger.h>


namespace glow {

void ConsoleLogger::handle(const LogMessage& message)
{
	std::cout << levelString(message.level()) << message.message() << std::endl;
}

std::string ConsoleLogger::levelString(LogMessage::Level level)
{
	switch (level)
	{
	case LogMessage::Fatal:
		return "#fatal: ";
	case LogMessage::Critical:
		return "#critical: ";
	case LogMessage::Warning:
		return "#warning: ";
	default:
		return "";
	}
}

} // namespace glow
