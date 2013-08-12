#pragma once

#ifdef __APPLE__

#include <set>

#include "AbstractNativeWindow.h"


namespace glow 
{

class MacWindow : public AbstractNativeWindow
{
public:
    MacWindow();
    virtual ~MacWindow();


protected:
    static std::set<MacWindow*> s_windows;
};

} // namespace glow

#endif
