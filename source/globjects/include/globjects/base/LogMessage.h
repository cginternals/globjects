#pragma once

#include <string>

#include <globjects/globjects_api.h>

#include <globjects/base/LogMessageLevel.h>

namespace globjects
{

/** \brief Encapsulates a simple log message and its severity level.

	LogMessages are handled and dispatched by the global logging handler which 
    has to be a subclass of AbstractLogHandler.

	\see logging.h
	\see AbstractLogHandler
*/
class GLOBJECTS_API LogMessage
{
public:
    LogMessage(LogMessageLevel level, const std::string & message);

    LogMessageLevel level() const;
    const std::string & message() const;

protected:
    LogMessageLevel m_level;
    std::string m_message;
};

} // namespace globjects
