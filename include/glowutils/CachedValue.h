
#pragma once

namespace glow
{

template <typename T>
class CachedValue
{
public:
	CachedValue();
	CachedValue(const T & value);

	bool isValid() const;
	
	T & value();
	const T & value() const;
	void setValue(const T & value);

	void validate();
	void invalidate();

protected:
	bool m_valid;
	T m_value;
};

} // namespace glow

#include <glowutils/CachedValue.hpp>
