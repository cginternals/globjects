
#pragma once


#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/Program.h>


namespace globjects
{


class ProgramBinary;
class AbstractStringSource;


class AbstractProgramBinaryImplementation
{
public:
    AbstractProgramBinaryImplementation();
    virtual ~AbstractProgramBinaryImplementation();

    static AbstractProgramBinaryImplementation * get(Program::BinaryImplementation impl =
        Program::BinaryImplementation::GetProgramBinaryARB);

    virtual bool updateProgramLinkSource(const Program * program) const = 0;
    virtual std::unique_ptr<ProgramBinary> getProgramBinary(const Program * program) const = 0;
};


} // namespace globjects
