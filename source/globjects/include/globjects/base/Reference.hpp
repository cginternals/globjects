#pragma once

#include <globjects/base/Reference.h>

namespace globjects
{

template<typename T>
Reference<T>::Reference(T * _reference)
: m_reference(_reference)
{
}

template<typename T>
Reference<T>::~Reference()
{
}

template<typename T>
T * Reference<T>::reference() const
{
    return m_reference;
}

template<typename T>
void Reference<T>::setReference(T * reference)
{
    m_reference = reference;
}

} // namespace globjects
