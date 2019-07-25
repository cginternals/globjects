
#pragma once


#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/ProgramPipeline.h>


namespace globjects
{


class AbstractProgramPipelineImplementation
{
public:
    AbstractProgramPipelineImplementation();
    virtual ~AbstractProgramPipelineImplementation();

    static AbstractProgramPipelineImplementation * get(ProgramPipeline::Implementation impl =
        ProgramPipeline::Implementation::Legacy);

    virtual gl::GLuint create() const = 0;
    virtual void destroy(gl::GLuint id) const = 0;

    virtual void use(const ProgramPipeline * pipeline) const = 0;
    virtual void release() const = 0;
    virtual void useStages(const ProgramPipeline * pipeline, const Program * program, gl::UseProgramStageMask stages) const = 0;
    virtual void releaseStages(const ProgramPipeline * pipeline, gl::UseProgramStageMask stages) const = 0;
    virtual void validate(const ProgramPipeline * pipeline) const = 0;
    virtual gl::GLint getInt(const ProgramPipeline * pipeline, gl::GLenum pname) const = 0;
    virtual std::string getInfoLog(const ProgramPipeline * pipeline) const = 0;
};


} // namespace globjects
