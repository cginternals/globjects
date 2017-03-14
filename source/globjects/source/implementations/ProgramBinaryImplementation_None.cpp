
#include "ProgramBinaryImplementation_None.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Program.h>
#include <globjects/ProgramBinary.h>


namespace globjects 
{


bool ProgramBinaryImplementation_None::updateProgramLinkSource(const Program * program) const
{
    return program->compileAttachedShaders();
}

std::unique_ptr<ProgramBinary> ProgramBinaryImplementation_None::getProgramBinary(const Program * /*program*/) const
{
    return nullptr;
}


} // namespace globjects
