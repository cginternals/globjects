#include <globjects/base/ConsoleLogger.h>

#include <iostream>

#include <globjects/base/LogMessage.h>
#include <globjects/base/LogMessageLevel.h>

namespace globjects
{

void ConsoleLogger::handle(const LogMessage& message)
{
    if (LogMessageLevel::Info > message.level())
        std::cerr << levelString(message.level()) << message.message() << std::endl;
    else
        std::cout << levelString(message.level()) << message.message() << std::endl;
}

std::string ConsoleLogger::levelString(const LogMessageLevel level)
{
    switch (level)
    {
    case LogMessageLevel::Fatal:
        return "#fatal: ";
    case LogMessageLevel::Critical:
        return "#critical: ";
    case LogMessageLevel::Warning:
        return "#warning: ";
    default:
        return "";
    }
}

} // namespace globjects
