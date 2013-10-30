#pragma once

#include <cassert>

namespace glow
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
void CachedValue<T>::setValue(const T & value)
{
	m_valid = true;
	m_value = value;
}

template <typename T>
void CachedValue<T>::validate()
{
	m_valid = true;
}

template <typename T>
void CachedValue<T>::invalidate()
{
	m_valid = false;
}

} // namespace glow
