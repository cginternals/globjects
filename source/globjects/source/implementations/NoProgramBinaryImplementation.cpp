#include "NoProgramBinaryImplementation.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Program.h>

namespace glo {

bool NoProgramBinaryImplementation::updateProgramLinkSource(const Program* program) const
{
    return program->compileAttachedShaders();
}

ProgramBinary* NoProgramBinaryImplementation::getProgramBinary(const Program* /*program*/) const
{
    return nullptr;
}

} // namespace glo
