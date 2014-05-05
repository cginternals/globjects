#include <glowbase/formatString.h>

#include <string>
#include <sstream>
#include <set>
#include <locale>
#include <iomanip>

#include <iostream>

namespace glow
{

int readInt(const char* str, int& number)
{
	number = 0;
	int read = 0;
	char c;
	while (isdigit(c = *str++))
	{
		number = 10 * number + (c - '0');
		read++;
	}

	return read;
}

void parseFormat(std::ostream& stream, const char*& format)
{
    std::locale loc;

	static std::set<char> alignmentFlags = { 'l', 'r', 'i' };
	while (alignmentFlags.find(*format) != alignmentFlags.end())
	{
		char alignment = *format++;

		switch (alignment)
		{
			case 'l':
				stream << std::left;
				break;
			case 'r':
				stream << std::right;
				break;
			case 'i':
				stream << std::internal;
				break;
		}
	}

	static std::set<char> flags = { 'a', '+', ' ', '#', 'u', 'p', '0' };
	while (flags.find(std::tolower(*format, loc)) != flags.end())
	{
		char flag = *format++;

		switch (flag)
		{
			case 'a':
				stream << std::boolalpha;
				break;
			case '+':
				stream << std::showpos;
				break;
			case ' ':
				stream << std::skipws;
				break;
			case '#':
				stream << std::showbase;
				break;
			case 'u':
				stream << std::uppercase;
				break;
			case 'p':
				stream << std::showpoint;
				break;
			case '0':
				stream << std::setfill('0');
				break;
		}
	}

	static std::set<char> floatFieldFlags = { 'f', 'e' };
	if (floatFieldFlags.find(std::tolower(*format, loc)) != floatFieldFlags.end())
	{
		char flag = *format++;

		if (std::isupper(flag, loc))
		{
			stream << std::uppercase;
		}

		switch (std::tolower(flag, loc))
		{
			case 'f':
				stream << std::fixed;
				break;
			case 'e':
				stream << std::scientific;
				break;
		}
	}

	if (*format == '?')
	{
		char fill = *++format;
		stream << std::setfill(fill);
		++format;
	}

	int width;
	format += readInt(format, width);
	if (width>0)
	{
		stream << std::setw(width);
	}

	if (*format == '.')
	{
		int precision;
		format += readInt(++format, precision);
		if (precision>0)
		{
			stream << std::setprecision(precision);
		}
	}

	static std::set<char> baseFlags = { 'd', 'o', 'x' };
	if (baseFlags.find(*format) != baseFlags.end())
	{
		char baseFlag = *format++;

		if (std::isupper(baseFlag, loc))
		{
			stream << std::uppercase;
		}

		switch (std::tolower(baseFlag, loc))
		{
			case 'd':
				stream << std::dec;
				break;
			case 'o':
				stream << std::oct;
				break;
			case 'x':
				stream << std::hex;
				break;
		}
	}

	while (*format && *format++ != ';');
}

void streamprintf(std::ostream& stream, const char* format)
{
	stream << format;
}

} // namespace glowbase
