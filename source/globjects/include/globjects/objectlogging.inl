
#pragma once


#include <globjects/Uniform.h>


namespace globjects
{


template <typename T>
LogMessageBuilder operator<<(LogMessageBuilder builder, const Uniform<T> * uniform)
{
    builder << "Uniform" << "(";
    if (uniform->identity().isName())
        builder << uniform->identity().name();
    else
        builder << uniform->identity().location();

    builder << ", " << uniform->value() << ")";

    return builder;
}

template <typename T>
LogMessageBuilder operator<<(LogMessageBuilder builder, Uniform<T> * uniform)
{
	return operator<<(builder, const_cast<const Uniform<T>*>(uniform));
}


} // namespace globjects
