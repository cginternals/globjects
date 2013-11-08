#include <cassert>

#include <glow/ConsoleLogger.h>

#include <glow/logging.h>

namespace glow 
{

namespace 
{
LogMessage::Level g_verbosityLevel = LogMessage::Info;
AbstractLogHandler * g_logHandler = new ConsoleLogger();
}

LogMessageBuilder info(LogMessage::Level level)
{
	return LogMessageBuilder(level, level <= g_verbosityLevel ? g_logHandler : nullptr);
}

LogMessageBuilder debug()
{
	return info(LogMessage::Debug);
}

LogMessageBuilder warning()
{
	return info(LogMessage::Warning);
}

LogMessageBuilder critical()
{
	return info(LogMessage::Critical);
}

LogMessageBuilder fatal()
{
	return info(LogMessage::Fatal);
}

AbstractLogHandler* loggingHandler()
{
	return g_logHandler;
}

void setLoggingHandler(AbstractLogHandler* handler)
{
	delete g_logHandler;
	g_logHandler = handler;
}

GLOW_API void setVerbosityLevel(LogMessage::Level verbosity)
{
	g_verbosityLevel = verbosity;
}

GLOW_API LogMessage::Level verbosityLevel()
{
	return g_verbosityLevel;
}

} // namespace glow
