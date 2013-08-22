#include <glow/formatString.h>

namespace glow 
{

template <typename... Arguments> void info(const char* format, Arguments... arguments)
{
	info() << formatString(format, arguments...);
}

template <typename... Arguments> void debug(const char* format, Arguments... arguments)
{
	debug() << formatString(format, arguments...);
}

template <typename... Arguments> void warning(const char* format, Arguments... arguments)
{
	warning() << formatString(format, arguments...);
}

template <typename... Arguments> void critical(const char* format, Arguments... arguments)
{
	critical() << formatString(format, arguments...);
}

template <typename... Arguments> void fatal(const char* format, Arguments... arguments)
{
	fatal() << formatString(format, arguments...);
}

} // namespace glow