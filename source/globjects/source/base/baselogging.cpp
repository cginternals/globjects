#include <globjects/base/baselogging.h>

#include <globjects/base/AbstractLogHandler.h>
#include <globjects/base/LogMessageBuilder.h>
#include <globjects/base/ConsoleLogger.h>

namespace
{
    globjects::LogMessageLevel l_verbosityLevel = globjects::LogMessageLevel::Info;
    globjects::AbstractLogHandler * l_logHandler = new globjects::ConsoleLogger();
}

namespace globjects
{

LogMessageBuilder info(const LogMessageLevel level)
{
    return LogMessageBuilder(level, level <= l_verbosityLevel ? l_logHandler : nullptr);
}

LogMessageBuilder debug()
{
    return info(LogMessageLevel::Debug);
}

LogMessageBuilder warning()
{
    return info(LogMessageLevel::Warning);
}

LogMessageBuilder critical()
{
    return info(LogMessageLevel::Critical);
}

LogMessageBuilder fatal()
{
    return info(LogMessageLevel::Fatal);
}

AbstractLogHandler * loggingHandler()
{
    return l_logHandler;
}

void setLoggingHandler(AbstractLogHandler* handler)
{
    delete l_logHandler;
    l_logHandler = handler;
}

void setVerbosityLevel(const LogMessageLevel verbosity)
{
    l_verbosityLevel = verbosity;
}

LogMessageLevel verbosityLevel()
{
    return l_verbosityLevel;
}

} // namespace globjects
