#pragma once

#include <glow/AbstractLogHandler.h>

namespace glow {

/** \brief The ConsoleLogger class writes LogMessages to stdout.

	This is the default LogMessage handler of glow.

	\see setLoggingHandler
	\see logging.h
*/
class ConsoleLogger : public AbstractLogHandler
{
public:
	void handle(const LogMessage& message) override;
protected:
	std::string levelString(LogMessage::Level level);
};

} // namespace glow
