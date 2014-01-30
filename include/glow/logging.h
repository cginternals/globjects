#pragma once

#include <glow/glow.h>
#include <glow/AbstractLogHandler.h>
#include <glow/LogMessageBuilder.h>

namespace glow
{

/**
  * Creates a stream like object (LogMessageBuilder) to create a LogMessage from the objects
  * passed to it and sends the LogMessage to the global LogMessageHandler when the builder goes out of scope.
  * Similar to `qDebug()` from Qt.
  *
  * \code{.cpp}
  * info() << "Message: " << 3.14;
  * \endcode
  */
GLOW_API LogMessageBuilder info(LogMessage::Level level = LogMessage::Info);
GLOW_API LogMessageBuilder debug();
GLOW_API LogMessageBuilder warning();
GLOW_API LogMessageBuilder critical();
GLOW_API LogMessageBuilder fatal();

GLOW_API void setLoggingHandler(AbstractLogHandler * handler);
GLOW_API AbstractLogHandler * loggingHandler();

GLOW_API void setVerbosityLevel(LogMessage::Level verbosity);
GLOW_API LogMessage::Level verbosityLevel();

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
GLOW_API void info(const char* format, Arguments... arguments);

/**
 *  \see info
 */
template <typename... Arguments>
GLOW_API void debug(const char* format, Arguments... arguments);

/**
 *  \see info
 */
template <typename... Arguments>
GLOW_API void warning(const char* format, Arguments... arguments);

/**
 *  \see info
 */
template <typename... Arguments>
GLOW_API void critical(const char* format, Arguments... arguments);

/**
 *  \see info
 */
template <typename... Arguments>
GLOW_API void fatal(const char* format, Arguments... arguments);


} // namespace glow

#include <glow/logging.hpp>
