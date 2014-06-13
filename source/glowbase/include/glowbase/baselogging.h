#pragma once

#include <glowbase/glowbase_api.h>

#include <glowbase/LogMessage.h>
#include <glowbase/LogMessageBuilder.h>

namespace glow
{

class AbstractLogHandler;
class LogMessageBuilder;

/**
  * Creates a stream like object (LogMessageBuilder) to create a LogMessage from the objects
  * passed to it and sends the LogMessage to the global LogMessageHandler when the builder goes out of scope.
  * Similar to `qDebug()` from Qt.
  *
  * \code{.cpp}
  * info() << "Message: " << 3.14;
  * \endcode
  */
GLOWBASE_API LogMessageBuilder info(LogMessage::Level level = LogMessage::Info);
GLOWBASE_API LogMessageBuilder debug();
GLOWBASE_API LogMessageBuilder warning();
GLOWBASE_API LogMessageBuilder critical();
GLOWBASE_API LogMessageBuilder fatal();

GLOWBASE_API void setLoggingHandler(AbstractLogHandler * handler);
GLOWBASE_API AbstractLogHandler * loggingHandler();

GLOWBASE_API void setVerbosityLevel(LogMessage::Level verbosity);
GLOWBASE_API LogMessage::Level verbosityLevel();

/**
 * Uses formatString to write on the usual logging streams.
 *
 *  Sample usage:
 *  \code{.cpp}
 *      info("This is a test: %; pi = %+0E10.5;", 42, 3.141592653589793); // output: "This is a test: 42 pi = +3.14159E+00"
 *      info("%; - %X; - %rf?_10.2;", "a string", 255, 2.71828182846); // output: "a string - 255 - ______2.72"
 *  \endcode
 *
 *   \see formatString
 */
template <typename... Arguments>
void info(const char* format, Arguments... arguments);

/**
 *  \see info
 */
template <typename... Arguments>
void debug(const char* format, Arguments... arguments);

/**
 *  \see info
 */
template <typename... Arguments>
void warning(const char* format, Arguments... arguments);

/**
 *  \see info
 */
template <typename... Arguments>
void critical(const char* format, Arguments... arguments);

/**
 *  \see info
 */
template <typename... Arguments>
void fatal(const char* format, Arguments... arguments);

} // namespace glow

#include <glowbase/baselogging.hpp>
