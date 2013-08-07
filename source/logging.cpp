#include <glow/logging.h>

#include <glow/ConsoleLogger.h>

namespace glow {

namespace {

LoggingInterface* logHandler = new ConsoleLogger();

}

LogMessageBuilder log(LogMessage::Level level)
{
	return LogMessageBuilder(level);
}

LogMessageBuilder debug()
{
	return log(LogMessage::Debug);
}

LogMessageBuilder warning()
{
	return log(LogMessage::Warning);
}

LogMessageBuilder error()
{
	return log(LogMessage::Error);
}

LogMessageBuilder fatal()
{
	return log(LogMessage::Fatal);
}

LoggingInterface* loggingHandler()
{
	return logHandler;
}

void setLoggingHandler(LoggingInterface* handler)
{
	delete logHandler;
	logHandler = handler;
}

} // namespace glow
