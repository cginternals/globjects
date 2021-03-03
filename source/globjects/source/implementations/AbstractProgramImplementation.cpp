
#include "AbstractProgramImplementation.h"

#include <globjects/globjects.h>
#include <glbinding/gl/extension.h>

#include "ProgramImplementation_Legacy.h"


using namespace gl;


namespace globjects 
{


AbstractProgramImplementation::AbstractProgramImplementation()
{
}

AbstractProgramImplementation::~AbstractProgramImplementation()
{
}

AbstractProgramImplementation * AbstractProgramImplementation::get(const Program::Implementation /*impl*/)
{
    return ProgramImplementation_Legacy::instance();
}


} // namespace globjects
