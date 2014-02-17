#include <glowutils/Timer.h>

namespace glowutils
{

Timer::Timer(bool start, bool autoUpdate)
:   m_paused(true)
,   m_auto(autoUpdate)
,   m_t0(clock::now())
,   m_offset(0.0L)
,   m_elapsed(0.0L)
{
    m_t1 = m_t0;
    m_tp = m_t0;

    if(start)
        this->start();
}

Timer::~Timer()
{
}

void Timer::update() const
{
    m_t1 = m_paused ? m_tp : clock::now();

    const auto delta = m_t1 - m_t0;  
    m_elapsed = nano(delta).count() + m_offset;
}

bool Timer::paused() const
{
    return m_paused;
}

void Timer::start()
{
    if(!m_paused)
        return;

    const time_point t2 = clock::now();
    m_offset -= nano(t2 - m_tp).count();

    m_t1 = t2;
    m_paused = false;
}

void Timer::pause()
{
    if(m_paused)
        return;

    m_tp = clock::now();
    m_paused = true;
}

void Timer::stop()
{
    pause();
    reset();
}

void Timer::reset()
{
    m_offset = 0.0L;
    m_elapsed = 0.0L;

    m_t0 = clock::now();
    m_t1 = m_t0;
    m_tp = m_t0;
}

long double Timer::elapsed() const
{
    if(m_auto)
        update();

    return m_elapsed;
}

void Timer::setAutoUpdating(const bool auto_update)
{
    m_auto = auto_update;
}

bool Timer::autoUpdating() const
{
    return m_auto;
}

} // namespace glowutils
