#pragma once

#include <glowwindow/glowwindow.h>
#include <glow/Referenced.h>

namespace glow 
{

class Window;
class WindowEvent;
class KeyEvent;
class MouseEvent;
class ScrollEvent;
class ResizeEvent;
class PaintEvent;
class FocusEvent;
class IconifyEvent;
class MoveEvent;
class TimerEvent;

class GLOWWINDOW_API WindowEventHandler : public Referenced
{
public:
    WindowEventHandler();
    virtual ~WindowEventHandler();

    void handleEvent(WindowEvent & event);

    /** initialize is called only once when starting to run a window.
        The window's context is made current before and done current afterwards.
    */
    virtual void initialize(Window & window);
    
    /** finalize is called only once, just before a window returns
        from running. The window's context is made current before and done 
        current afterwards.
    */
    virtual void finalize(Window & window);

    /** The idle event is called when no painting within the attached window's
        main message loop is called. This might happen for fixed repaint time
        intervals.
    */
    virtual void idle(Window & window);

protected:

    /** The window's context is made current before and done current afterwards.
    */
    virtual void resizeEvent(ResizeEvent & event);
    virtual void framebufferResizeEvent(ResizeEvent & event);

    virtual void moveEvent(MoveEvent & event);

    /** Swap buffers gets called afterwards, and window's context is made 
        current before and done current afterwards.
    */
    virtual void paintEvent(PaintEvent & event);

    virtual void keyPressEvent(KeyEvent & event);
    virtual void keyReleaseEvent(KeyEvent & event);

    virtual void mousePressEvent(MouseEvent & event);
    virtual void mouseMoveEvent(MouseEvent & event);
    virtual void mouseReleaseEvent(MouseEvent & event);

    virtual void scrollEvent(ScrollEvent & event);

    virtual void focusEvent(FocusEvent & event);
    virtual void iconifyEvent(IconifyEvent & event);

    virtual void timerEvent(TimerEvent & event);
};

} // namespace glow
