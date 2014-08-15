#pragma once

#include <globjects-window/WindowEventHandler.h>
#include <globjects-utils/Timer.h>
#include <globjects-window/events.h>

#include <string>

class ExampleWindowEventHandler : public glowindow::WindowEventHandler
{
public:
    ExampleWindowEventHandler();
    virtual ~ExampleWindowEventHandler();

    virtual void initialize(glowindow::Window & window) override;

    virtual void handleEvent(glowindow::WindowEvent & event) override;
protected:
    gloutils::Timer m_timer;
    long double m_swapElapsedTime;
    unsigned int m_swapCount;
    std::string m_baseTitle;

    void setViewport(glowindow::ResizeEvent & event);
    void computeFps(glowindow::PaintEvent & event);
    void handleDefaultKeys(glowindow::KeyEvent & event);
};
