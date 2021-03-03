
#include "ProgramPipelineImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/ProgramPipeline.h>
#include <globjects/Program.h>


using namespace gl;


namespace globjects 
{

GLuint ProgramPipelineImplementation_Legacy::create() const
{
    gl::GLuint id;
    glGenProgramPipelines(1, &id);

    return id;
}

void ProgramPipelineImplementation_Legacy::destroy(const GLuint id) const
{
    glDeleteProgramPipelines(1, &id);
}

void ProgramPipelineImplementation_Legacy::use(const ProgramPipeline * pipeline) const
{
    gl::glBindProgramPipeline(pipeline->id());
}

void ProgramPipelineImplementation_Legacy::release() const
{
    gl::glBindProgramPipeline(0);
}

void ProgramPipelineImplementation_Legacy::useStages(const ProgramPipeline * pipeline, const Program * program, gl::UseProgramStageMask stages) const
{
    gl::glUseProgramStages(pipeline->id(), stages, program->id());
}

void ProgramPipelineImplementation_Legacy::releaseStages(const ProgramPipeline * pipeline, gl::UseProgramStageMask stages) const
{
    gl::glUseProgramStages(pipeline->id(), stages, 0);
}

void ProgramPipelineImplementation_Legacy::validate(const ProgramPipeline * pipeline) const
{
    gl::glValidateProgramPipeline(pipeline->id());
}

gl::GLint ProgramPipelineImplementation_Legacy::getInt(const ProgramPipeline * pipeline, gl::GLenum pname) const
{
    gl::GLint value = 0;
    gl::glGetProgramPipelineiv(pipeline->id(), pname, &value);

    return value;
}

std::string ProgramPipelineImplementation_Legacy::getInfoLog(const ProgramPipeline * pipeline) const
{
    gl::GLint length = getInt(pipeline, gl::GL_INFO_LOG_LENGTH);

    if (length == 0)
    {
        return std::string();
    }

    std::vector<char> log(length);

    gl::glGetProgramPipelineInfoLog(pipeline->id(), length, &length, log.data());

    return std::string(log.data(), length);
}


} // namespace globjects
