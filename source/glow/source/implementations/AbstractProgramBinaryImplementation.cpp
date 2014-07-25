#include "AbstractProgramBinaryImplementation.h"

#include <glow/glow.h>

#include "NoProgramBinaryImplementation.h"
#include "GetProgramBinaryImplementation.h"

namespace glow {

AbstractProgramBinaryImplementation::AbstractProgramBinaryImplementation()
{
}

AbstractProgramBinaryImplementation::~AbstractProgramBinaryImplementation()
{
}

AbstractProgramBinaryImplementation * AbstractProgramBinaryImplementation::create()
{
    if (hasExtension(gl::GLextension::GL_ARB_get_program_binary))
    {
        return new GetProgramBinaryImplementation();
    }
    else
    {
        return new NoProgramBinaryImplementation();
    }
}

} // namespace glow
