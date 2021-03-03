
#include "SamplerImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/Renderbuffer.h>


using namespace gl;


namespace globjects 
{

GLuint SamplerImplementation_Legacy::create() const
{
    gl::GLuint id;
    glGenSamplers(1, &id);

    return id;
}

void SamplerImplementation_Legacy::destroy(const GLuint id) const
{
    glDeleteSamplers(1, &id);
}


} // namespace globjects
