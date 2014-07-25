#pragma once

#include <glbinding/gl/types.h>

namespace glow
{

class Program;
class ProgramBinary;

class AbstractProgramBinaryImplementation
{
public:
    AbstractProgramBinaryImplementation();
    virtual ~AbstractProgramBinaryImplementation();

    static AbstractProgramBinaryImplementation * create();

    virtual bool updateProgramLinkSource(const Program* program) const = 0;
    virtual ProgramBinary* getProgramBinary(const Program* program) const = 0;
};

} // namespace glow
