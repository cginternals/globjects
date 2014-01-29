#pragma once

#include <glowutils/CachedValue.h>

#include <cassert>

namespace glowutils
{

template <typename T>
CachedValue<T>::CachedValue()
: m_valid(false)
, m_value(T())
{
}

template <typename T>
CachedValue<T>::CachedValue(const T & value)
: m_valid(true)
, m_value(value)
{
}

template <typename T>
bool CachedValue<T>::isValid() const
{
	return m_valid;
}

template <typename T>
T & CachedValue<T>::value()
{
    return m_value;
}

template <typename T>
const T & CachedValue<T>::value() const
{
	assert(m_valid);
	return m_value;
}

template <typename T>
void CachedValue<T>::setValue(const T & value, const bool validate) const
{
    m_valid = validate;
	m_value = value;
}

template <typename T>
void CachedValue<T>::validate() const
{
	m_valid = true;
}

template <typename T>
void CachedValue<T>::invalidate() const
{
	m_valid = false;
}

} // namespace glowutils
