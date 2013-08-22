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


/** \brief Foo bar

	These functions take a format string and any number of arguments, which will be sent to the logging handler.
	The format string is similar to that of printf, but only translates to stream manipulators, as the types can be inferred because it is a template.

	\see http://www.cplusplus.com/reference/ios/ios_base/fmtflags/
	Format syntax:

	general:
	%[alignment flag][independent flags][floatfield flag][fill character][width][precision][base];
	specific:
	%[ l | r | i ][ a | # | + | u | p ]*[ e | f ][?<fill character>][<width>][.<precision>][ d | o | x ];

	alignment flags:
		l - left
		r - right
		i - internal

	independent flags:
		a - boolalpha
		# - showbase
		+ - showpos
		u - uppercase
		p - showpoint
		0 - setfill(0) [redundant]

	floatfield flag:
		e - scientific
		f - fixed

	fill character:
		- setfill

	base:
		d - decimal
		o - octal
		x - hexadecimal

	In addition, if the base or floatfield flag is uppercase, it will automatically enable the uppercase flag without 'u'.
	Note: To end a format specifier, you have to add a semicolon.
	%% will escape a % character.

	Sample usage:
	\code{.cpp}
		debug("This is a test: %; pi = %+0E10.5;", 42, 3.141592653589793);
	\endcode

	\see debug
	\see warning
	\see critical
	\see fatal
*/
template <typename... Arguments> GLOW_API void info(const char* format, Arguments... arguments);
template <typename... Arguments> GLOW_API void debug(const char* format, Arguments... arguments);
template <typename... Arguments> GLOW_API void warning(const char* format, Arguments... arguments);
template <typename... Arguments> GLOW_API void critical(const char* format, Arguments... arguments);
template <typename... Arguments> GLOW_API void fatal(const char* format, Arguments... arguments);

} // namespace glow

#include <glow/logging.hpp>
