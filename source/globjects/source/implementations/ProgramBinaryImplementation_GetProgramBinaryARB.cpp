
#include "ProgramBinaryImplementation_GetProgramBinaryARB.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/Program.h>
#include <globjects/ProgramBinary.h>


using namespace gl;

namespace globjects 
{

bool ProgramBinaryImplementation_GetProgramBinaryARB::updateProgramLinkSource(const Program * program) const
{
    if (program->m_binary)
    {
        glProgramBinary(program->id(), program->m_binary->format(), program->m_binary->data(), program->m_binary->length());
        return true;
    }

    return program->compileAttachedShaders();
}

ProgramBinary * ProgramBinaryImplementation_GetProgramBinaryARB::getProgramBinary(const Program * program) const
{
    int length = program->get(GL_PROGRAM_BINARY_LENGTH);

    if (length == 0)
        return nullptr;

    GLenum format;
    std::vector<char> binary(length);

    glGetProgramBinary(program->id(), length, nullptr, &format, binary.data());

    return new ProgramBinary(format, binary);
}

} // namespace globjects
