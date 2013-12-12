#pragma once

#include <ostream>

#include <glow/glow.h>

namespace glow 
{

GLOW_API int readInt(const char* str, int& number);
GLOW_API void parseFormat(std::ostream& stream, const char*& format);

GLOW_API void streamprintf(std::ostream& stream, const char* format);
template <typename T, typename... Args>
void streamprintf(std::ostream& stream, const char* format, const T& value, Args... args);

template <typename... Args>
std::string formatString(const char* format, Args... args);

} // namespace glow

#include <glow/formatString.hpp>
