#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include <glow/glow.h>


namespace glow 
{

class GLOW_API Screen
{
#ifdef WIN32
    typedef LPCTSTR ScreenHandle;
#endif

public:
    static const unsigned int getNumScreens();
    

private:
    Screen();
    ~Screen();
};

} // namespace glow
