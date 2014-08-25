#include "AbstractProgramBinaryImplementation.h"

#include <globjects/globjects.h>

#include "NoProgramBinaryImplementation.h"
#include "GetProgramBinaryImplementation.h"

namespace glo {

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

} // namespace glo
