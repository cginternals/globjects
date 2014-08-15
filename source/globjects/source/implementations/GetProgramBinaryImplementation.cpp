#include "GetProgramBinaryImplementation.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/Program.h>

namespace glo {

bool GetProgramBinaryImplementation::updateProgramLinkSource(const Program* program) const
{
    if (program->m_binary)
    {
        gl::glProgramBinary(program->id(), program->m_binary->format(), program->m_binary->data(), program->m_binary->length());

        return true;
    }

    return program->compileAttachedShaders();
}

ProgramBinary* GetProgramBinaryImplementation::getProgramBinary(const Program* program) const
{
    int length = program->get(gl::GL_PROGRAM_BINARY_LENGTH);

    if (length == 0)
    {
        return nullptr;
    }

    gl::GLenum format;
    std::vector<char> binary(length);

    gl::glGetProgramBinary(program->id(), length, nullptr, &format, binary.data());

    return new ProgramBinary(format, binary);
}

} // namespace glo
