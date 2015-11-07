#pragma once

#include <string>

#include <globjects/globjects_api.h>

#include <globjects/base/AbstractLogHandler.h>

namespace globjects
{

class LogMessage;
enum class LogMessageLevel : unsigned int;

/** \brief Writes LogMessages to stdout.

    This is the default LogMessage handler of globjects.

	\see setLoggingHandler
	\see logging.h
*/
class GLOBJECTS_API ConsoleLogger : public AbstractLogHandler
{
public:
    virtual void handle(const LogMessage & message) override;

protected:
    static std::string levelString(LogMessageLevel level);
};

} // namespace globjects
