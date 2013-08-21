#pragma once

#include <glow/AbstractLogHandler.h>


namespace glow 
{

GLOW_API LogMessageBuilder info(LogMessage::Level level = LogMessage::Info);
GLOW_API LogMessageBuilder debug();
GLOW_API LogMessageBuilder warning();
GLOW_API LogMessageBuilder critical();
GLOW_API LogMessageBuilder fatal();

GLOW_API void setLoggingHandler(AbstractLogHandler * handler);
GLOW_API AbstractLogHandler * loggingHandler();

GLOW_API void setVerbosityLevel(LogMessage::Level verbosity);
GLOW_API LogMessage::Level verbosityLevel();

} // namespace glow
