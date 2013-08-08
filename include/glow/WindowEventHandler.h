#pragma once

#include <glow/glow.h>


namespace glow 
{

class GLOW_API WindowEventHandler
{
public:
    WindowEventHandler();
    virtual ~WindowEventHandler();

    /** The initialize event is called only once when starting to run a window.
    */
    virtual void initializeEvent();
    
    /** The initialize event is called only once, just before a window returns from running.
    */
    virtual void deinitializeEvent();

    //virtual void activateEvent();
    //virtual void minimizeEvent();

    virtual void resizeEvent(
        const unsigned int width
    ,   const unsigned int height);

    virtual void paintEvent();
    virtual void idleEvent();
};

} // namespace glow
