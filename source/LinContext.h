#pragma once

#ifdef __linux__

#include "AbstractNativeContext.h"


namespace glow 
{

class LinContext : public AbstractNativeContext
{
public:
    LinContext();
    virtual ~LinContext();

};

} // namespace glow

#endif
