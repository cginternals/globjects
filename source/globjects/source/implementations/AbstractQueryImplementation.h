
#pragma once


#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/Query.h>


namespace globjects
{


class AbstractQueryImplementation
{
public:
    AbstractQueryImplementation();
    virtual ~AbstractQueryImplementation();

    static AbstractQueryImplementation * get(Query::Implementation impl =
        Query::Implementation::Legacy);

    virtual gl::GLuint create() const = 0;
    virtual void destroy(gl::GLuint id) const = 0;
};


} // namespace globjects
