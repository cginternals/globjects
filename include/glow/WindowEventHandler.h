#pragma once

#include <glow/glow.h>


namespace glow 
{

class Window;

class GLOW_API WindowEventHandler
{
public:
    WindowEventHandler();
    virtual ~WindowEventHandler();

    virtual void attachEvent(Window * window);
    virtual void dettachEvent(Window * window);

    virtual void closeEvent();
    virtual void destroyEvent();

    virtual void resizeEvent(
        const unsigned int width
    ,   const unsigned int height);

protected:
    Window * m_window;
};

} // namespace glow
