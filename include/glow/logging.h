#pragma once

#include <glow/LoggingInterface.h>


namespace glow {

GLOW_API LogMessageBuilder log(LogMessage::Level level = LogMessage::Debug);

GLOW_API LogMessageBuilder debug();
GLOW_API LogMessageBuilder warning();
GLOW_API LogMessageBuilder error();
GLOW_API LogMessageBuilder fatal();

GLOW_API LoggingInterface* loggingHandler();
GLOW_API void setLoggingHandler(LoggingInterface* handler);

} // namespace glow
