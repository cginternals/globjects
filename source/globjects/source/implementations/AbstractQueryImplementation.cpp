
#include "AbstractQueryImplementation.h"

#include <globjects/globjects.h>
#include <glbinding/gl/extension.h>

#include "QueryImplementation_Legacy.h"


using namespace gl;


namespace globjects 
{


AbstractQueryImplementation::AbstractQueryImplementation()
{
}

AbstractQueryImplementation::~AbstractQueryImplementation()
{
}

AbstractQueryImplementation * AbstractQueryImplementation::get(const Query::Implementation /*impl*/)
{
    return QueryImplementation_Legacy::instance();
}


} // namespace globjects
