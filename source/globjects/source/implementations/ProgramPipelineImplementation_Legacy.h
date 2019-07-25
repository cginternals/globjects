
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

    virtual void use(const ProgramPipeline * pipeline) const override;
    virtual void release() const override;
    virtual void useStages(const ProgramPipeline * pipeline, const Program * program, gl::UseProgramStageMask stages) const override;
    virtual void releaseStages(const ProgramPipeline * pipeline, gl::UseProgramStageMask stages) const override;
    virtual void validate(const ProgramPipeline * pipeline) const override;
    virtual gl::GLint getInt(const ProgramPipeline * pipeline, gl::GLenum pname) const override;
    virtual std::string getInfoLog(const ProgramPipeline * pipeline) const override;
};


} // namespace globjects
