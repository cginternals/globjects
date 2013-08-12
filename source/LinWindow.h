#pragma once

#ifdef __linux__

#include <set>

#include "AbstractNativeWindow.h"


namespace glow 
{

class LinWindow : public AbstractNativeWindow
{
public:
    LinWindow();
    virtual ~LinWindow();


protected:
    static std::set<LinWindow*> s_windows;
};

} // namespace glow

#endif
