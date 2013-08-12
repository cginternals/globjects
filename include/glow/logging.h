#pragma once

#include <glow/LoggingInterface.h>


namespace glow {

GLOW_API LogMessageBuilder info(LogMessage::Level level = LogMessage::Info);
GLOW_API LogMessageBuilder debug();
GLOW_API LogMessageBuilder warning();
GLOW_API LogMessageBuilder critical();
GLOW_API LogMessageBuilder fatal();

GLOW_API LoggingInterface* loggingHandler();
GLOW_API void setLoggingHandler(LoggingInterface* handler);
GLOW_API void setVerbosityLevel(LogMessage::Level severity);
GLOW_API LogMessage::Level verbosityLevel();

} // namespace glow
