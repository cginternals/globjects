#pragma once

#include <string>

#include <globjects/base/Referenced.h>

#include <common/Timer.h>


class Window;
class WindowEvent;
class KeyEvent;
class MouseEvent;
class MouseEnterEvent;
class MouseLeaveEvent;
class ScrollEvent;
class ResizeEvent;
class PaintEvent;
class FocusEvent;
class IconifyEvent;
class MoveEvent;
class TimerEvent;

/** \brief Can be attached to a Window to handle events.
    The window's context is made current before calling any methods and done current afterwards.
*/
class WindowEventHandler : public globjects::Referenced
{
public:
    WindowEventHandler();
    virtual ~WindowEventHandler();

    virtual void handleEvent(WindowEvent & event);

    /** initialize is called only once when starting to run a window.
    */
    virtual void initialize(Window & window);
    
    /** finalize is called only once, just before a window returns from running.
    */
    virtual void finalize(Window & window);

    /** idle is called when no events were queued in the current iteration of the main loop.
    */
    virtual void idle(Window & window);

protected:

    virtual void resizeEvent(ResizeEvent & event);
    virtual void framebufferResizeEvent(ResizeEvent & event);

    virtual void moveEvent(MoveEvent & event);

    /** Swap buffers gets called afterwards.
    */
    virtual void paintEvent(PaintEvent & event);

    virtual void keyPressEvent(KeyEvent & event);
    virtual void keyReleaseEvent(KeyEvent & event);

    virtual void mousePressEvent(MouseEvent & event);
    virtual void mouseMoveEvent(MouseEvent & event);
    virtual void mouseReleaseEvent(MouseEvent & event);

    virtual void mouseEnterEvent(MouseEnterEvent & event);
    virtual void mouseLeaveEvent(MouseLeaveEvent & event);

    virtual void scrollEvent(ScrollEvent & event);

    virtual void focusEvent(FocusEvent & event);
    virtual void iconifyEvent(IconifyEvent & event);

    virtual void timerEvent(TimerEvent & event);

protected:
    void computeFps(PaintEvent & event);

protected:
    Timer m_timer;

    long double m_swapElapsedTime;
    unsigned int m_swapCount;

    std::string m_baseTitle;
};
