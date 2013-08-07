#include <glow/ConsoleLogger.h>

#include <iostream>

namespace glow {

void ConsoleLogger::handle(const LogMessage& message)
{
	std::cout << levelString(message.level()) << message.message() << std::endl;
}

std::string ConsoleLogger::levelString(LogMessage::Level level)
{
	switch (level)
	{
		case LogMessage::Warning:
			return "#warning: ";
		case LogMessage::Error:
			return "#error: ";
		case LogMessage::Fatal:
			return "#fatal: ";
		default:
			return "";
	}
}

} // namespace glow
