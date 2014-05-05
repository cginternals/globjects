#pragma once

#include <glowbase/formatString.h>

#include <sstream>
#include <cassert>

namespace glow
{

template <typename T, typename... Args>
void streamprintf(std::ostream& stream, const char* format, const T& value, Args... args)
{
    assert(format != nullptr);

	while (*format)
	{
		if (*format == '%' && *++format != '%')
		{
			auto flags = stream.flags();
			parseFormat(stream, format);
			stream << value;
			stream.flags(flags);
			streamprintf(stream, format, args...);
			return;
		}
		else
			stream << *format++;
	}
}

template <typename... Args>
std::string formatString(const char* format, Args... args)
{
    assert(format != nullptr);

	std::stringstream ss;
	streamprintf(ss, format, args...);
	return ss.str();
}

} // namespace glowbase
