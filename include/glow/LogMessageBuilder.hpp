#pragma once

namespace glow
{

template <typename T>
LogMessageBuilder& LogMessageBuilder::operator<< (T * t_pointer)
{
	return *this << static_cast<void*>(t_pointer);
}

} // namespace glow
