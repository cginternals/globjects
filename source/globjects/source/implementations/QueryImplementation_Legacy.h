
#pragma once


#include <memory>

#include "../base/Singleton.h"

#include "AbstractQueryImplementation.h"


namespace globjects
{


class QueryImplementation_Legacy : public AbstractQueryImplementation
    , public Singleton<QueryImplementation_Legacy>
{
public:
    virtual gl::GLuint create() const override;
    virtual void destroy(gl::GLuint id) const override;
};


} // namespace globjects
