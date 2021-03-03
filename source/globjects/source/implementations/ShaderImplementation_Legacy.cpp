
#include "ShaderImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/Program.h>


using namespace gl;


namespace globjects 
{

GLuint ShaderImplementation_Legacy::create(gl::GLenum type) const
{
    return glCreateShader(type);
}

void ShaderImplementation_Legacy::destroy(const GLuint id) const
{
    glDeleteShader(id);
}


} // namespace globjects
