
#include "ProgramPipelineImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/ProgramPipeline.h>


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


} // namespace globjects
