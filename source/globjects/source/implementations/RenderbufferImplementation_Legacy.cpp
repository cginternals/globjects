
#include "RenderbufferImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/Renderbuffer.h>


using namespace gl;


namespace globjects 
{

GLuint RenderbufferImplementation_Legacy::create() const
{
    gl::GLuint id;
    glGenRenderbuffers(1, &id);

    return id;
}

void RenderbufferImplementation_Legacy::destroy(const GLuint id) const
{
    glDeleteRenderbuffers(1, &id);
}


} // namespace globjects
