#pragma once

#ifdef __APPLE__

#include "AbstractNativeContext.h"


namespace glow 
{

class MacContext : public AbstractNativeContext
{
public:
    MacContext();
    virtual ~MacContext();
};

} // namespace glow

#endif
