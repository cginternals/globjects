
#pragma once


#include <globjects/globjects_api.h>

#include <globjects/base/LogMessage.h>
#include <globjects/base/LogMessageBuilder.h>


namespace globjects
{


class AbstractLogHandler;


/**
  * Creates a stream like object (LogMessageBuilder) to create a LogMessage from the objects
  * passed to it and sends the LogMessage to the global LogMessageHandler when the builder goes out of scope.
  * Similar to `qDebug()` from Qt.
  *
  * \code{.cpp}
  * info() << "Message: " << 3.14;
  * \endcode
  */
GLOBJECTS_API LogMessageBuilder info(LogMessageLevel level = LogMessageLevel::Info);
GLOBJECTS_API LogMessageBuilder debug();
GLOBJECTS_API LogMessageBuilder warning();
GLOBJECTS_API LogMessageBuilder critical();
GLOBJECTS_API LogMessageBuilder fatal();

GLOBJECTS_API void setLoggingHandler(AbstractLogHandler * handler);
GLOBJECTS_API AbstractLogHandler * loggingHandler();

GLOBJECTS_API void setVerbosityLevel(LogMessageLevel verbosity);
GLOBJECTS_API LogMessageLevel verbosityLevel();


} // namespace globjects
