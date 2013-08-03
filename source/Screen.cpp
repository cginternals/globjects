
#include <glow/Log.h>

#include <glow/Screen.h>

#include <stdio.h>


namespace glow
{

const unsigned int Screen::getNumScreens()
{
    unsigned int numScreens = 0;

#ifdef WIN32

    // http ://msdn.microsoft.com/en-us/library/windows/desktop/dd162609(v=vs.85).aspx

    DISPLAY_DEVICE device;

    ZeroMemory(&device, sizeof(DISPLAY_DEVICE));
    device.cb = sizeof(DISPLAY_DEVICE);

    DWORD iDevNum = 0;
    while (EnumDisplayDevices(NULL, iDevNum, &device, 0))
    {
        DISPLAY_DEVICE screen;

        ZeroMemory(&screen, sizeof(DISPLAY_DEVICE));
        screen.cb = sizeof(DISPLAY_DEVICE);

        DWORD iScreenNum = 0;
        while (EnumDisplayDevices(device.DeviceName, iScreenNum, &screen, 0))
            ++iScreenNum;

        numScreens += iScreenNum;
        ++iDevNum;
    }

#else
#endif

    return numScreens;
}

} // namespace glow
