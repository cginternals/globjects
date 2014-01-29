#pragma once

#include <glow/Array.h>

#include <cassert>

namespace glow
{

template<typename T>
Array<T>::Array()
{
}

template<typename T>
Array<T>::Array(size_t count)
: std::vector<T>(count)
{
}

template<typename T>
Array<T>::Array(size_t count, const T& value)
: std::vector<T>(count, value)
{
}

template<typename T>
Array<T>::Array(const Array<T> & array)
:   std::vector<T>(array)
{
}

template<typename T>
Array<T>::Array(const std::vector<T> & vector)
:   std::vector<T>(vector)
{
}

template<typename T>
Array<T>::Array(const std::initializer_list<T> & list)
:   std::vector<T>(list)
{
}

template<typename T>
Array<T>::Array(
    T * data
,   const unsigned size)
:   std::vector<T>(data, data + size)
{
    assert(data != nullptr);
}

template<typename T>
Array<T>::Array(
    void * data
,   const unsigned size)
:   std::vector<T>(reinterpret_cast<T*>(data), reinterpret_cast<T*>(data) + size / sizeof(T))
{
    assert(data != nullptr);
}

template<typename T>
Array<T>::Array(Array&& other) NOEXCEPT
: std::vector<T>(other)
{
}

template<typename T>
const void * Array<T>::rawData() const
{
    return reinterpret_cast<const void*>(std::vector<T>::data());
}

template<typename T>
GLint Array<T>::rawSize() const
{
    return static_cast<GLint>(std::vector<T>::size() * sizeof(T));
}

template<typename T>
Array<T> & Array<T>::operator<<(const T & element)
{
    std::vector<T>::push_back(element);
    return * this;
}

} // namespace glow
