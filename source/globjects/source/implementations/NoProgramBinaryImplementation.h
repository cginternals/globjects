#pragma once

#include "AbstractProgramBinaryImplementation.h"

namespace glo
{

class NoProgramBinaryImplementation : public AbstractProgramBinaryImplementation
{
public:
    virtual bool updateProgramLinkSource(const Program* program) const override;
    virtual ProgramBinary* getProgramBinary(const Program* program) const override;
};

} // namespace glo
