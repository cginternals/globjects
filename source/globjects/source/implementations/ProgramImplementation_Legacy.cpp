
#include "ProgramImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/Program.h>


using namespace gl;


namespace globjects 
{

GLuint ProgramImplementation_Legacy::create() const
{
    return glCreateProgram();
}

void ProgramImplementation_Legacy::destroy(const GLuint id) const
{
    glDeleteProgram(id);
}


} // namespace globjects
