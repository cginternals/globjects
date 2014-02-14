#pragma once

#include <glow/LogMessageBuilder.h>

#include <cassert>

namespace glow
{

template <typename T>
LogMessageBuilder& LogMessageBuilder::operator<<(Uniform<T>* uniform)
{  
    assert(uniform != nullptr);

    *this << "Uniform (" << uniform->name() << ", " << uniform->value() << ")";

    return *this;
}

template <typename T>
LogMessageBuilder& LogMessageBuilder::operator<<(ref_ptr<T> ref_pointer)
{
    return *this << ref_pointer.get();
}

template <typename T>
LogMessageBuilder& LogMessageBuilder::operator<< (T * pointer)
{
    return *this << static_cast<void*>(pointer);
}

template <typename T>
LogMessageBuilder& LogMessageBuilder::operator<<(const Array<T>& array)
{
    *this << "Array(";
    for (size_t i = 0; i < array.size(); ++i)
    {
        *this << array[i];
        if (i < array.size()-1)
            *this << ", ";
    }
    *this << ")";

    return *this;
}

template <typename T>
LogMessageBuilder& LogMessageBuilder::operator<<(const std::vector<T>& vector)
{
    *this << "vector(";
    for (size_t i = 0; i < vector.size(); ++i)
    {
        *this << vector[i];
        if (i < vector.size()-1)
            *this << ", ";
    }
    *this << ")";

    return *this;
}

template <typename T, std::size_t Count>
LogMessageBuilder& LogMessageBuilder::operator<<(const std::array<T, Count>& array)
{
    *this << "array(";
    for (size_t i = 0; i < Count; ++i)
    {
        *this << array[i];
        if (i < Count-1)
            *this << ", ";
    }
    *this << ")";

    return *this;
}

} // namespace glow
