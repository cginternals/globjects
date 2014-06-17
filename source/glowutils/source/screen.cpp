#include <glowutils/screen.h>

#include <cstdio>
#include <set>

#include <glow/logging.h>

#ifdef WIN32
#include <windows.h>
#endif


namespace glowutils
{

namespace screen
{

unsigned int getNumScreens()
{
    unsigned int numScreens = 0;

#ifdef WIN32

    numScreens = GetSystemMetrics(SM_CMONITORS);

#elif __APPLE__

#else

#endif

    return numScreens;
}

void getPhysicalSize(
    unsigned int & width
,   unsigned int & height)
{
    width  = 0; 
    height = 0;

#ifdef WIN32

    const HWND hWnd = GetDesktopWindow();
    const HDC dc    = GetDC(hWnd);

    if (0 == dc)
    {
        glow::warning() << "Could not't get a device context from the desktop window.";
        return;
    }

    width  = GetDeviceCaps(dc, HORZSIZE);
    height = GetDeviceCaps(dc, VERTSIZE);

    if(0 == ReleaseDC(hWnd, dc))
        glow::warning() << "Releasing temporary (query) device context failed.";

#elif __APPLE__

#else

#endif 
}

void getDesktopResolution(
    unsigned int & width
,   unsigned int & height)
{
    width  = 0;
    height = 0;

#ifdef WIN32

    width  = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

#elif __APPLE__

#else

    // http://content.gpwiki.org/index.php/OpenGL:Tutorials:Setting_up_Opengl::glon_X11

#endif 
}

void getMaximumResolution(
    unsigned int & width
,   unsigned int & height)
{
    int area = 0;

    Resolutions resolutions;
    getValidResolutions(resolutions);

    for (auto & res : resolutions)
    {
        const int a = res.first * res.second;
        if (a <= area)
            continue;

        width  = res.first;
        height = res.second;
    }
}

void getPixelDensity(
    float & ppiHorizontal
,   float & ppiVertical)
{
    unsigned int wmm, hmm;
    unsigned int wpx, hpx;

    getPhysicalSize(wmm, hmm);
    getMaximumResolution(wpx, hpx);

    ppiHorizontal = 25.4f * static_cast<float>(wpx) / static_cast<float>(wmm);
    ppiVertical   = 25.4f * static_cast<float>(hpx) / static_cast<float>(hmm);
}

void getValidResolutions(Resolutions & resolutions)
{
    resolutions.clear();

#ifdef WIN32

    std::map<unsigned int, std::set<unsigned int>> map;

    DEVMODE dm;
    ZeroMemory(&dm, sizeof(DEVMODE));

    dm.dmSize = sizeof(DEVMODE);

    int iModeNum = 0;
    while (EnumDisplaySettings(NULL, iModeNum, &dm))
    {
        ++iModeNum;

        auto f = map.find(dm.dmPelsWidth);
        if (f != map.cend() && f->second.find(dm.dmPelsHeight) != f->second.cend())
            continue;

        if (f != map.cend())
            f->second.insert(dm.dmPelsHeight);
        else
            map[dm.dmPelsWidth].insert(dm.dmPelsHeight);

        resolutions.insert(std::pair<unsigned int, unsigned int>(dm.dmPelsWidth, dm.dmPelsHeight));
    }

#elif __APPLE__

#else

#endif 
}

} // namespace screen

} // namespace glow
