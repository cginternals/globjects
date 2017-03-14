
#pragma once


#include <memory>

#include "../base/Singleton.h"

#include "AbstractProgramBinaryImplementation.h"


namespace globjects
{


class AbstractStringSource;


class ProgramBinaryImplementation_GetProgramBinaryARB : public AbstractProgramBinaryImplementation
    , public Singleton<ProgramBinaryImplementation_GetProgramBinaryARB>
{
public:
    virtual bool updateProgramLinkSource(const Program * program) const override;
    virtual std::unique_ptr<ProgramBinary> getProgramBinary(const Program * program) const override;
};


} // namespace globjects
