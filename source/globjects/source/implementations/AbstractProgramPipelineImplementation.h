
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
};


} // namespace globjects
