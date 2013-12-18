#pragma once

#include <cassert>

#include <glow/LogMessageBuilder.h>

namespace glow
{

template <typename T>
LogMessageBuilder& LogMessageBuilder::operator<<(Uniform<T>* uniform)
{
    assert(uniform != nullptr);

    *this << static_cast<AbstractUniform*>(uniform) << " " << uniform->value();
}

template <typename T>
LogMessageBuilder& LogMessageBuilder::operator<<(ref_ptr<T> ref_pointer)
{
    return *this<<ref_pointer.get();
}

template <typename T>
LogMessageBuilder& LogMessageBuilder::operator<< (T * pointer)
{
    return *this << static_cast<void*>(pointer);
}

} // namespace glow
