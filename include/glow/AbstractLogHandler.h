#pragma once

#include <glow/glow.h>
#include <glow/LogMessage.h>


namespace glow {

/** \brief The AbstractLogHandler class is an abstract interface to handle LogMessages.
 *
 * glow uses a global logging handler to dispatch all generated LogMessages.
 * This is the abstract interface for handling them.
 * Subclass this class if you wish to replace the global default logging behavior in glow, which is to write everything to stdout.
 *
 * \see setLoggingHandler
 * \see logging.h
 */
class GLOW_API AbstractLogHandler
{
public:
	virtual ~AbstractLogHandler() 
    {
    }

	virtual void handle(const LogMessage& message) = 0;
};

} // namespace glow
