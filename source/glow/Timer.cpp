
#include "ChronoTimer.h"

#include <glow/Timer.h>


namespace glow
{

Timer::Timer(
    const bool start
,   const bool autoUpdate)
:   m_chrono(new ChronoTimer(start, autoUpdate))
{
}

Timer::~Timer()
{
    delete m_chrono;
}

void Timer::update() const
{
    m_chrono->update();
}

const bool Timer::paused() const
{
    return m_chrono->paused();
}

void Timer::start()
{
    m_chrono->start();
}

void Timer::pause()
{
    m_chrono->pause();
}

void Timer::stop()
{
    m_chrono->stop();
}

void Timer::reset()
{
    m_chrono->reset();
}

const long double Timer::elapsed() const
{
    return m_chrono->elapsed();
}

void Timer::setAutoUpdating(const bool auto_update)
{
    m_chrono->setAutoUpdating(auto_update);
}

const bool Timer::autoUpdating() const
{
    return m_chrono->autoUpdating();
}

} // namespace glow
