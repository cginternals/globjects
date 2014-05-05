#pragma once

#include <glow/objectlogging.h>
#include <glow/glmlogging.h>
#include <glow/Uniform.h>

namespace glow {

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

} // namespace glow
