
#pragma once


#include <memory>

#include "../base/Singleton.h"

#include "AbstractProgramPipelineImplementation.h"


namespace globjects
{


class ProgramPipelineImplementation_Legacy : public AbstractProgramPipelineImplementation
    , public Singleton<ProgramPipelineImplementation_Legacy>
{
public:
    virtual gl::GLuint create() const override;
    virtual void destroy(gl::GLuint id) const override;
};


} // namespace globjects
