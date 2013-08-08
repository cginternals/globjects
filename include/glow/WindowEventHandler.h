#pragma once

#include <glow/glow.h>


namespace glow 
{

class KeyEvent;

class GLOW_API WindowEventHandler
{
public:
    WindowEventHandler();
    virtual ~WindowEventHandler();

    /** The initialize event is called only once when starting to run a window.
        The window's context is made current before and done current afterwards.
    */
    virtual void initializeEvent();
    
    /** The initialize event is called only once, just before a window returns 
        from running. The window's context is made current before and done 
        current afterwards.
    */
    virtual void deinitializeEvent();

    //virtual void activateEvent();
    //virtual void minimizeEvent();

    /** The window's context is made current before and done current afterwards.
    */
    virtual void resizeEvent(
        const unsigned int width
    ,   const unsigned int height);

    /** Swap buffers gets called afterwards, and window's context is made 
        current before and done current afterwards.
    */
    virtual void paintEvent();

    /** The idle event is called when no painting within the attached window's 
        main message loop is called. This might happen for fixed repaint time 
        intervals.
    */
    virtual void idleEvent();


    virtual void keyPressEvent(KeyEvent & event);
    virtual void keyReleaseEvent(KeyEvent & event);
};

} // namespace glow
