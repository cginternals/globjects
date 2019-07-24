
#include "AbstractRenderbufferImplementation.h"

#include <globjects/globjects.h>
#include <glbinding/gl/extension.h>

#include "RenderbufferImplementation_Legacy.h"


using namespace gl;


namespace globjects 
{


AbstractRenderbufferImplementation::AbstractRenderbufferImplementation()
{
}

AbstractRenderbufferImplementation::~AbstractRenderbufferImplementation()
{
}

AbstractRenderbufferImplementation * AbstractRenderbufferImplementation::get(const Renderbuffer::Implementation /*impl*/)
{
    return RenderbufferImplementation_Legacy::instance();
}


} // namespace globjects
