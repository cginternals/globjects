#pragma once

#include <globjects-utils/Timer.h>

#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <string>

class ExampleWindowEventHandler : public WindowEventHandler
{
public:
    ExampleWindowEventHandler();
    virtual ~ExampleWindowEventHandler();

    virtual void initialize(Window & window) override;

    virtual void handleEvent(WindowEvent & event) override;
protected:
    gloutils::Timer m_timer;
    long double m_swapElapsedTime;
    unsigned int m_swapCount;
    std::string m_baseTitle;

    void setViewport(ResizeEvent & event);
    void computeFps(PaintEvent & event);
    void handleDefaultKeys(KeyEvent & event);
};
