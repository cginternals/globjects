#pragma once

#include <glbinding/gl/types.h>

#include <globjects/Program.h>


namespace globjects
{

class Program;
class ProgramBinary;

class AbstractProgramBinaryImplementation
{
public:
    AbstractProgramBinaryImplementation();
    virtual ~AbstractProgramBinaryImplementation();

    static AbstractProgramBinaryImplementation * get(Program::BinaryImplementation impl =
        Program::BinaryImplementation::GetProgramBinaryARB);

    virtual bool updateProgramLinkSource(const Program * program) const = 0;
    virtual ProgramBinary* getProgramBinary(const Program * program) const = 0;
};

} // namespace globjects
