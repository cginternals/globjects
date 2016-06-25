
#pragma once

#include <iosfwd>
#include <string>

#include <globjects/globjects_api.h>


namespace globjects
{


GLOBJECTS_API int readInt(const char* str, int& number);
GLOBJECTS_API void parseFormat(std::ostream& stream, const char*& format);

GLOBJECTS_API void streamprintf(std::ostream& stream, const char* format);

/**
 * Format a number of arguments and prints them to a stream.
 *
 * \see formatString
 */
template <typename T, typename... Args>
void streamprintf(std::ostream& stream, const char* format, const T& value, Args... args);

/**
 * This function takes a format string and any number of arguments of different types.
 * It works similar to printf, but is safer as it can infer the types.
 * This also means that the type does not need to be encoded in the format string,
 * instead they only indicate stream manipulators.
 *
 * Format syntax
 * =============
 *
 * `%[alignment flag][independent flags][floatfield flag][fill character][width][precision][base];`
 * `%[ l | r | i ][ a | # | + | u | p ]*[ e | f ][?<fill character>][<width>][.<precision>][ d | o | x ];`
 *
 * alignment flags
 * ---------------
 *
 * flag | manipulator
 * ---- | -----
 * `l` | left
 * `r` | right
 * `i` | internal
 *
 * independent flags
 * -----------------
 *
 * flag | manipulator
 * ---- | -----
 * `a` | boolalpha
 * `#` | showbase
 * `+` | showpos
 * `u` | uppercase
 * `p` | showpoint
 * `0` | setfill(0) [redundant]
 *
 * floatfield flags
 * ----------------
 *
 * flag | manipulator
 * ---- | -----
 * `e` | scientific
 * `f` | fixed
 *
 * base
 * ----
 *
 * flag | manipulator
 * ---- | -----
 * `d` | decimal
 * `o` | octal
 * `x` | hexadecimal
 *
 * In addition, if the base or floatfield flag is uppercase, it will automatically enable the uppercase flag without 'u'.
 * Note: To end a format specifier, you have to add a semicolon.
 * `%%` will escape a % character.
 *
 * \see http://www.cplusplus.com/reference/ios/ios_base/fmtflags/
 */
template <typename... Args>
std::string formatString(const char* format, Args... args);


} // namespace globjects


#include <globjects/base/formatString.inl>
