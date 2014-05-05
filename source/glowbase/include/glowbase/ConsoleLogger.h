#pragma once

#include <glowbase/glowbase_api.h>
#include <glowbase/AbstractLogHandler.h>
#include <glowbase/LogMessage.h>

namespace glow
{

/** \brief Writes LogMessages to stdout.

	This is the default LogMessage handler of glow.

	\see setLoggingHandler
	\see logging.h
*/
class GLOWBASE_API ConsoleLogger : public AbstractLogHandler
{
public:
    virtual void handle(const LogMessage & message) override;

protected:
    static std::string levelString(LogMessage::Level level);
};

} // namespace glow
