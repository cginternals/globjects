
#include "QueryImplementation_Legacy.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>

#include <globjects/Query.h>


using namespace gl;


namespace globjects 
{

GLuint QueryImplementation_Legacy::create() const
{
    gl::GLuint id;
    glGenQueries(1, &id);

    return id;
}

void QueryImplementation_Legacy::destroy(const GLuint id) const
{
    glDeleteQueries(1, &id);
}


} // namespace globjects
