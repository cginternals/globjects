#pragma once

#include <globjects/base/Singleton.h>

#include "AbstractProgramBinaryImplementation.h"


namespace globjects
{

class ProgramBinaryImplementation_GetProgramBinaryARB : public AbstractProgramBinaryImplementation
    , public Singleton<ProgramBinaryImplementation_GetProgramBinaryARB>
{
public:
    virtual bool updateProgramLinkSource(const Program * program) const override;
    virtual ProgramBinary * getProgramBinary(const Program * program) const override;
};

} // namespace globjects
