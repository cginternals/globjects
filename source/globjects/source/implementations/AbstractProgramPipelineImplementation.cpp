
#include "AbstractProgramPipelineImplementation.h"

#include <globjects/globjects.h>
#include <glbinding/gl/extension.h>

#include "ProgramPipelineImplementation_Legacy.h"


using namespace gl;


namespace globjects 
{


AbstractProgramPipelineImplementation::AbstractProgramPipelineImplementation()
{
}

AbstractProgramPipelineImplementation::~AbstractProgramPipelineImplementation()
{
}

AbstractProgramPipelineImplementation * AbstractProgramPipelineImplementation::get(const ProgramPipeline::Implementation /*impl*/)
{
    return ProgramPipelineImplementation_Legacy::instance();
}


} // namespace globjects
