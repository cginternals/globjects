#pragma once

#include <glowwindow/glowwindow.h>


namespace glow 
{

class Window;
class KeyEvent;

class GLOWWINDOW_API WindowEventHandler
{
public:
    WindowEventHandler();
    virtual ~WindowEventHandler();

    /** The initialize event is called only once when starting to run a window.
        The window's context is made current before and done current afterwards.
    */
    virtual void initializeEvent(Window & window);
    
    /** The initialize event is called only once, just before a window returns 
        from running. The window's context is made current before and done 
        current afterwards.
    */
    virtual void deinitializeEvent(Window & window);

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
