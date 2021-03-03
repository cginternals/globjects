
#pragma once


#include <memory>

#include "../base/Singleton.h"

#include "AbstractProgramImplementation.h"


namespace globjects
{


class AbstractStringSource;


class ProgramImplementation_Legacy : public AbstractProgramImplementation
    , public Singleton<ProgramImplementation_Legacy>
{
public:
    virtual gl::GLuint create() const override;
    virtual void destroy(gl::GLuint id) const override;
};


} // namespace globjects
