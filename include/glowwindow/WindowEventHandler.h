#pragma once

#include <glowwindow/glowwindow.h>
#include <glow/Referenced.h>

namespace glow 
{

class Window;
class WindowEvent;
class KeyEvent;

class GLOWWINDOW_API WindowEventHandler : public Referenced
{
public:
    WindowEventHandler();
    virtual ~WindowEventHandler();

    void handleEvent(Window & window, WindowEvent * event);

    /** The initialize event is called only once when starting to run a window.
        The window's context is made current before and done current afterwards.
    */
    virtual void initialize(Window & window);
    
    /** The initialize event is called only once, just before a window returns 
        from running. The window's context is made current before and done 
        current afterwards.
    */
    virtual void finalize(Window & window);

    //virtual void activateEvent();
    //virtual void minimizeEvent();

    /** The window's context is made current before and done current afterwards.
    */
    virtual void resizeEvent(
        Window & window
    ,   unsigned int width
    ,   unsigned int height);

    /** Swap buffers gets called afterwards, and window's context is made 
        current before and done current afterwards.
    */
    virtual void paintEvent(Window & window);

    /** The idle event is called when no painting within the attached window's 
        main message loop is called. This might happen for fixed repaint time 
        intervals.
    */
    virtual void idleEvent(Window & window);


    virtual void keyPressEvent(
        Window & window
    ,   KeyEvent & event);

    virtual void keyReleaseEvent(
        Window & window
    ,   KeyEvent & event);
};

} // namespace glow
