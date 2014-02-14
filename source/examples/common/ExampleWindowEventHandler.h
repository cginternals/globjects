#pragma once

#include <glowwindow/WindowEventHandler.h>
#include <glowutils/Timer.h>
#include <glowwindow/events.h>

#include <string>

class ExampleWindowEventHandler : public glowwindow::WindowEventHandler
{
public:
    ExampleWindowEventHandler();
    virtual ~ExampleWindowEventHandler();

    virtual void handleEvent(glowwindow::WindowEvent & event) override;
protected:
    glowutils::Timer m_timer;
    long double m_swapElapsedTime;
    unsigned int m_swapCount;
    std::string m_baseTitle;

    void setViewport(glowwindow::ResizeEvent & event);
    void computeFps(glowwindow::PaintEvent & event);
    void handleDefaultKeys(glowwindow::KeyEvent & event);
};
